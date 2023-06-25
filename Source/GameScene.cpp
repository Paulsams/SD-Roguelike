#include "GameScene.h"

#include "Utils/TMXGenerator.h"
#include "Mobs/Factory/MobsFactoryFromConfig.h"
#include "UI/Canvas.h"
#include "WorldSystem/World.h"
#include "WorldSystem/RandomGeneratorWorldBuilder.h"
#include "WorldSystem/ReadFileWorldBuilder.h"

using namespace cocos2d;

std::pair<Scene*, GameScene*> GameScene::createScene()
{
    Scene* scene = Scene::create();
    
    Camera* camera = scene->getDefaultCamera();
    Size size = Director::getInstance()->getWinSize();
    camera->initOrthographic(size.width, size.height, 1, 100);
    camera->setPosition(0, 0);
    camera->setAnchorPoint(Vec2::ZERO);
    camera->setPositionZ(cameraZ);
    
    GameScene* gameScene = GameScene::create(camera);
    gameScene->scheduleUpdate();
    scene->addChild(gameScene);
    
    Director::getInstance()->getEventDispatcher()->removeCustomEventListeners(GLViewImpl::EVENT_WINDOW_RESIZED);
    Director::getInstance()->getEventDispatcher()->addCustomEventListener(GLViewImpl::EVENT_WINDOW_RESIZED,
        [gameScene, camera](Event*)
    {
        Size size = Director::getInstance()->getWinSize();

        const Vec2 position = gameScene->m_runningScene ? gameScene->m_player->getPosition() : camera->getPosition();
        camera->initOrthographic(size.width, size.height, 1, 100);
        camera->setPosition(gameScene->getViewPointCenter(position * gameScene->m_world->getScale()));
        camera->setPositionZ(cameraZ);

        gameScene->m_canvas->setContentSize(size);
    });

    return {scene, gameScene};
}

GameScene* GameScene::create(Camera* camera)
{
    auto* scenePointer = new (std::nothrow) GameScene(); 
    if (scenePointer && scenePointer->init(camera))
    {
        scenePointer->autorelease();
        return scenePointer; 
    }
    CC_SAFE_DELETE(scenePointer);
    return nullptr;
}

bool GameScene::init(Camera* camera)
{
    if (!Layer::init())
        return false;

    m_camera = camera;
    auto mobsConfig = std::make_shared<mob::MobsConfig>();
    const auto mobFactory = std::make_shared<mob::MobsFactoryFromConfig>(mobsConfig);

    m_worldTileConfig = std::make_shared<WorldTileConfig>("Resources/World.json");

    m_gameLoop = std::make_shared<GameLoop>();

    TMXMapInfo* mapInfo = RandomGeneratorWorldBuilder().setPath("Template.tmx").setConfig(m_worldTileConfig->getLevelsTileConfig().at(0)).setWidth(55).setHeight(55).setIterCount(4).build();

    Tilemap* tileMap = TileMapProxy::create(mapInfo);

    m_world = World::create(tileMap, mobFactory);
//    m_world = World::create(tileMap, mobFactory);
//     m_world->setScale(0.5f);

//     const std::string tilemapInXML = generateXMLForTMXTiledMap(tileMap);
//     std::ofstream outfile ("Resources/test.tmx");
//     outfile << tilemapInXML << std::endl;
//     outfile.close();

//    m_world = World::create(ReadFileWorldBuilder().setPath("Custom.tmx").build(), mobFactory);
//    m_world = World::create(RandomGeneratorWorldBuilder().setPath("Template.tmx").setConfig(m_worldTileConfig).setWidth(100).setHeight(100).setIterCount(5).build());
    m_world->setScale(1.5f);
    m_gameLoop->add(m_world);

    this->addChild(m_world);

    createPlayer();

    m_canvas = Canvas::create(m_world, m_player, m_gameLoop);
    m_canvas->setContentSize(Director::getInstance()->getWinSize());
    this->addChild(m_canvas, 1);

    const Size winSize = Director::getInstance()->getWinSize();
    const Point viewPoint = getViewPointCenter(m_player->getPosition() * m_world->getScale());
    m_world->updateCullingRect(Rect(viewPoint / m_world->getScale(), winSize));
    m_camera->setPosition(viewPoint);

    m_player->moved += movedPlayerDelegate;
    m_player->attacked += attackPlayerDelegate;

    m_world->bossCountChanged += [this](const std::vector<mob::Mob*>& bosses)
    {
        if (bosses.empty())
        {
            m_canvas->showRestartScreen("Win game!!!", restarted);
        }
    };

    std::shared_ptr<IStat> healthStat;
    if (m_player->getStats()->tryGet(HEALTH, healthStat))
    {
        healthStat->changed += [this](IStat::currentValue currentValue, IStat::changedValue, IStat::wantedChangeValue)
        {
            if (currentValue > 0.0f)
                return;
            
            m_runningScene = false;
            schedule([this](float)
            {
                m_gameLoop->step();
            }, 0.5f, "UpdateGameLoop");

            m_canvas->showRestartScreen("Game Over", restarted);
        };
    }
    
    return true;
}

void GameScene::update(float delta)
{
    Layer::update(delta);

    if (!m_runningScene)
    {
        m_player->moved -= movedPlayerDelegate;
        m_player->attacked -= attackPlayerDelegate;
        m_world->removePlayer(m_player);
        unscheduleUpdate();
        m_world->update();
    }
}

void GameScene::createPlayer()
{
    static const Size sizeRect = {32, 32};
    
    m_player = Player::create(m_world);
    m_player->setPositionOnMapWithoutNotify(m_world->getSpawnPoint());
    m_world->addPlayer(m_player);

    constexpr int templatePeopleId = 2210 - 1;
    m_player->setClothe(Player::TEMPLATE_PEOPLE, {Paths::toGameTileset,
        Rect(Vec2{templatePeopleId % 64, templatePeopleId / 64} * sizeRect.width, sizeRect)});

    constexpr int bibId = 2149 - 1;
    m_player->setClothe(Player::BIB, {Paths::toGameTileset,
        Rect(Vec2{bibId % 64, bibId / 64} * sizeRect.width, sizeRect)});

    constexpr int helmetId = 1957 - 1;
    m_player->setClothe(Player::HELMET, {Paths::toGameTileset,
        Rect(Vec2{helmetId % 64, helmetId / 64} * sizeRect.width, sizeRect)});
}

GameScene::GameScene()
    : movedPlayerDelegate([this](BaseEntity*, BaseEntity::OldPosition, BaseEntity::NewPosition endPosition)
    {
        static constexpr int moveCameraTag = 10;
        static constexpr float moveCameraTime = 0.4f;
        static constexpr float coefficientOffsetSize = 0.4f;

        const Size winSize = Director::getInstance()->getWinSize();
        Point viewPoint = getViewPointCenter(m_world->convertToMapSpace(endPosition) * m_world->getScale());
        m_camera->stopActionByTag(moveCameraTag);
                
        auto moveTo = MoveTo::create(moveCameraTime, {viewPoint.x, viewPoint.y, m_camera->getPositionZ()});
        moveTo->setTag(moveCameraTag);
        m_camera->runAction(moveTo);
                
        m_world->updateCullingRect(Rect((viewPoint / m_world->getScale() - winSize * coefficientOffsetSize / 2.0f),
            winSize * (1.0f + coefficientOffsetSize)));

        this->m_gameLoop->step();
    })
    , attackPlayerDelegate([this]() { m_gameLoop->step(); }) { }

Point GameScene::getViewPointCenter(Point position) const
{
    const Size winSize = Director::getInstance()->getWinSize();
    const Size mapSize = m_world->getSize();
    const Size tileSize = m_world->getTileSize();

    position.x += Canvas::widthRightPanel / 2;
    
    Point viewPoint = position - Point(winSize.width / 2, winSize.height / 2);
    viewPoint.x = std::clamp(viewPoint.x, 0.0f, mapSize.width * tileSize.width * m_world->getScale() -
        winSize.width + Canvas::widthRightPanel);
    viewPoint.y = std::clamp(viewPoint.y, -Canvas::heightBackpack,
        (mapSize.height * tileSize.height * m_world->getScale() - winSize.height));
    
    return viewPoint;
}
