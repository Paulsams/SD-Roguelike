#include "GameScene.h"
#include "Commands/IMoveCommand.h"
#include "Commands/MoveDirectionCommand.h"
#include "WorldSystem/World.h"
#include "WorldSystem/ReadFileWorld.h"

using namespace cocos2d;

Scene* GameScene::createScene()
{
    Scene* scene = Scene::create();
    
    Camera* camera = scene->getDefaultCamera();
    Size size = Director::getInstance()->getWinSize();
    camera->initOrthographic(size.width, size.height, 1, 100);
    camera->setPosition(0, 0);
    camera->setPositionZ(10);
    
    GameScene* layer = GameScene::create(camera);
    scene->addChild(layer);

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

    const auto readFileGenerator = std::make_shared<ReadFileWorld>();
    readFileGenerator->changePath("TileMap.tmx");
    m_world = World::create(readFileGenerator);
    this->addChild(m_world);
    
    if (!m_world->generate())
        return false;

    const std::shared_ptr<IMoveCommand> movePlayerCommand = std::make_shared<MoveDirectionCommand>(m_world);
    const std::shared_ptr<IAttackCommand> attackPlayerCommand;

    m_player = Player::create(movePlayerCommand, attackPlayerCommand);
    m_player->setPosition(m_world->getSpawnPoint());

    m_gameLoop->add(m_player);
    
    this->addChild(m_player);
    m_camera->setPosition(getViewPointCenter(m_player->getPosition()));

    m_player->moved += [this]()
    {
        const int moveCameraTag = 10;
        const float moveCameraTime = 0.7f;
        
        Point viewPoint = getViewPointCenter(m_player->getPosition());
        m_camera->stopActionByTag(moveCameraTag);
        
        auto moveTo = MoveTo::create(moveCameraTime, {viewPoint.x, viewPoint.y, m_camera->getPositionZ()});
        moveTo->setTag(moveCameraTag);
        m_camera->runAction(moveTo);
    };

    // auto label = Label::createWithSystemFont("Hello World", "Arial", 96);
    // label->setAnchorPoint(Vec2(0.0, 0.0));
    // this->addChild(label, 1);
    
    return true;
}

Point GameScene::getViewPointCenter(Point position)
{
    const Size winSize = Director::getInstance()->getWinSize();
    const Size mapSize = m_world->getSize();
    const Size tileSize = m_world->getTileSize();
    
    Point viewPoint = position - Point(winSize.width / 2, winSize.height / 2);
    viewPoint.x = std::clamp(viewPoint.x, 0.0f, mapSize.width * tileSize.width - winSize.width);
    viewPoint.y = std::clamp(viewPoint.y, 0.0f, mapSize.height * tileSize.height - winSize.height);

    return viewPoint;

    // int x = MAX(position.x, winSize.width / 2);
    // int y = MAX(position.y, winSize.height / 2);
    // x = MIN(x, (m_tilemap->getMapSize().width * this->m_tilemap->getTileSize().width) - winSize.width / 2);
    // y = MIN(y, (m_tilemap->getMapSize().height * m_tilemap->getTileSize().height) - winSize.height/2);
    // Point actualPosition = Point(x, y);
    //
    // Point centerOfView = Point(winSize.width / 2, winSize.height / 2);
    // Point viewPoint = actualPosition - centerOfView;
    // m_camera->setPosition(viewPoint);
}
