#include "GameScene.h"
#include "Movement/MoveDirection.h"
#include "UI/Canvas.h"
#include "WorldSystem/World.h"
#include "WorldSystem/ReadFileWorldBuilder.h"

using namespace cocos2d;

Scene* GameScene::createScene()
{
    Scene* scene = Scene::create();
    
    Camera* camera = scene->getDefaultCamera();
    Size size = Director::getInstance()->getWinSize();
    camera->initOrthographic(size.width, size.height, 1, 100);
    camera->setPosition(0, 0);
    camera->setPositionZ(cameraZ);
    
    GameScene* gameScene = GameScene::create(camera);
    scene->addChild(gameScene);
    
    Director::getInstance()->getEventDispatcher()->addCustomEventListener(GLViewImpl::EVENT_WINDOW_RESIZED,
        [gameScene, camera](Event*)
    {
        Size size = Director::getInstance()->getWinSize();
        camera->initOrthographic(size.width, size.height, 1, 100);
        camera->setPosition(gameScene->getViewPointCenter(gameScene->m_player->getPosition()));
        camera->setPositionZ(cameraZ);

        gameScene->m_canvas->setContentSize(size);
    });

    return scene;
}

GameScene* GameScene::create(Camera* camera)
{
    const auto scenePointer = new GameScene(); 
    if (scenePointer && scenePointer->init(camera))
    {
        scenePointer->autorelease();
        return scenePointer; 
    }
    delete scenePointer;
    return nullptr;
}

bool GameScene::init(Camera* camera)
{
    if (!Layer::init())
        return false;

    m_camera = camera;
    
    m_gameLoop = std::make_shared<GameLoop>();
    
    m_canvas = Canvas::create(m_gameLoop);
    m_canvas->setContentSize(Director::getInstance()->getWinSize());
    this->addChild(m_canvas, 1);
    
    m_world = World::create(ReadFileWorldBuilder().setPath("TileMap.tmx").build());
    this->addChild(m_world);

    const std::shared_ptr<IMovement> movePlayerCommand = std::make_shared<MoveDirection>(m_world);
    const std::shared_ptr<IAttack> attackPlayerCommand;

    m_player = Player::create(movePlayerCommand, attackPlayerCommand);
    m_player->setPosition(m_world->getSpawnPoint());

    m_gameLoop->add(m_player);
    
    this->addChild(m_player);
    m_camera->setPosition(getViewPointCenter(m_player->getPosition()));

    m_player->moved += [this]()
    {
        static constexpr int moveCameraTag = 10;
        static constexpr float moveCameraTime = 0.7f;
        
        Point viewPoint = getViewPointCenter(m_player->getPosition());
        m_camera->stopActionByTag(moveCameraTag);
        
        auto moveTo = MoveTo::create(moveCameraTime, {viewPoint.x, viewPoint.y, m_camera->getPositionZ()});
        moveTo->setTag(moveCameraTag);
        m_camera->runAction(moveTo);

        this->m_gameLoop->step();
    };
    
    return true;
}

Point GameScene::getViewPointCenter(Point position) const
{
    const Size winSize = Director::getInstance()->getWinSize();
    const Size mapSize = m_world->getSize();
    const Size tileSize = m_world->getTileSize();
    
    Point viewPoint = position - Point(winSize.width / 2, winSize.height / 2);
    viewPoint.x = std::clamp(viewPoint.x, 0.0f, mapSize.width * tileSize.width - winSize.width + Canvas::widthRightPanel);
    viewPoint.y = std::clamp(viewPoint.y, 0.0f, mapSize.height * tileSize.height - winSize.height);
    
    return viewPoint;
}
