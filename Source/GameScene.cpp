#include "GameScene.h"

#include "ItemsSystem/AttackSearch/AttackSearchFromDFS.h"
#include "ItemsSystem/DealingDamage/SimpleDamage.h"
#include "UI/Canvas.h"
#include "WorldSystem/FunctionVisitorEntities.h"
#include "WorldSystem/World.h"
#include "WorldSystem/RandomGeneratorWorldBuilder.h"
#include "WorldSystem/ReadFileWorldBuilder.h"

using namespace cocos2d;

Scene* GameScene::createScene()
{
    Scene* scene = Scene::create();
    
    Camera* camera = scene->getDefaultCamera();
    Size size = Director::getInstance()->getWinSize();
    camera->initOrthographic(size.width, size.height, 1, 100);
    camera->setPosition(0, 0);
    camera->setAnchorPoint(Vec2::ZERO);
    camera->setPositionZ(cameraZ);
    
    GameScene* gameScene = GameScene::create(camera);
    scene->addChild(gameScene);
    
    Director::getInstance()->getEventDispatcher()->addCustomEventListener(GLViewImpl::EVENT_WINDOW_RESIZED,
        [gameScene, camera](Event*)
    {
        Size size = Director::getInstance()->getWinSize();
        camera->initOrthographic(size.width, size.height, 1, 100);
        camera->setPosition(gameScene->getViewPointCenter(gameScene->m_player->getPosition()
            * gameScene->m_world->getScale()));
        camera->setPositionZ(cameraZ);

        gameScene->m_canvas->setContentSize(size);
    });

    return scene;
}

GameScene* GameScene::create(Camera* camera)
{
    GameScene* scenePointer = new (std::nothrow) GameScene(); 
    if (scenePointer && scenePointer->init(camera))
    {
        scenePointer->autorelease();
        return scenePointer; 
    }
    CC_SAFE_DELETE(scenePointer);
    return nullptr;
}

TilemapConfig getTilemapConfig(const LevelTileConfig& levelConfig)
{
    std::unordered_map<int, TileType> tiles;
    for (auto tile : levelConfig.getGround())
        tiles.insert({tile, TileType::GROUND});
    for (auto tile : levelConfig.getRareGround())
        tiles.insert({tile, TileType::GROUND});
    for (auto tile : levelConfig.getWalls())
        tiles.insert({tile, TileType::OBSTACLE});
    for (auto tile : levelConfig.getRareWalls())
        tiles.insert({tile, TileType::OBSTACLE});
    for (auto tile : levelConfig.getObstacles())
        tiles.insert({tile, TileType::OBSTACLE});
    tiles.insert({0, TileType::OBSTACLE});
    return {tiles};
}

bool GameScene::init(Camera* camera)
{
    if (!Layer::init())
        return false;

    m_camera = camera;

    m_worldTileConfig = std::make_shared<WorldTileConfig>("Resources/World.json");

    const TilemapConfig tilemapConfig = getTilemapConfig(m_worldTileConfig->getLevelsTileConfig()[0]);
    
    m_gameLoop = std::make_shared<GameLoop>();
    m_world = World::create(ReadFileWorldBuilder().setPath("Custom.tmx").build(), tilemapConfig);
    //m_world = World::create(RandomGeneratorWorldBuilder().setPath("Template.tmx").setConfig(m_worldTileConfig).setWidth(100).setHeight(100).setIterCount(5).build(),
        // tilemapConfig);
    m_world->setScale(1.5f);
    this->addChild(m_world);

    AttackInfo::PossibleAttackDelegate dontHitObstacle =
        [](TileType tileType) { return tileType == TileType::GROUND; };
    
    AttackInfo::PossibleAttackFromEntity hitOnlyEnemies = FunctionVisitorEntitiesBuilder<bool>().
        setMob([](mob::Mob*) { return true; }).build();

    std::vector<Vec2Int> oneDistanceRanges = {{0, 1}};

    const auto delegateDamage = std::make_shared<DelegateDamage>();

    const auto defaultAttackInfo = std::make_shared<AttackInfo>(dontHitObstacle, hitOnlyEnemies,
        nullptr, std::make_shared<AttackSearchFromDFS>());

    const auto defaultAttack = AttackHandlerBuilder().addAttackData(oneDistanceRanges,
        std::make_shared<AttackWithDamage>(defaultAttackInfo, delegateDamage)).build();

    Weapon* defaultWeapon = Weapon::create(m_world, World::getRectFromGid(2943), WEAPON, defaultAttack,
        delegateDamage);
    defaultWeapon->setPositionOnMap({3, 3});
    m_world->addEntity(defaultWeapon);

    m_player = Player::create(m_world);
    m_gameLoop->add(m_player);

    m_world->addChild(m_player);
    m_world->addPlayer(m_player);
    m_player->setPositionOnMap(m_world->getSpawnPoint());

    m_canvas = Canvas::create(m_world, m_player, m_gameLoop);
    m_canvas->setContentSize(Director::getInstance()->getWinSize());
    this->addChild(m_canvas, 1);

    Size winSize = Director::getInstance()->getWinSize();
    Point viewPoint = getViewPointCenter(m_player->getPosition() * m_world->getScale());
    m_world->updateCullingRect(Rect(viewPoint, winSize));
    m_camera->setPosition(viewPoint);

    m_player->moved += [this](Vec2Int, Vec2Int)
    {
        static constexpr int moveCameraTag = 10;
        static constexpr float moveCameraTime = 0.4f;
        static constexpr float coefficientOffsetSize = 0.4f;

        Size winSize = Director::getInstance()->getWinSize();
        
        Point viewPoint = getViewPointCenter(m_player->getPosition() * m_world->getScale());
        m_camera->stopActionByTag(moveCameraTag);
        
        //m_world->moveTilemap(m_camera->getPosition() - viewPoint);
        
        auto moveTo = MoveTo::create(moveCameraTime, {viewPoint.x, viewPoint.y, m_camera->getPositionZ()});
        moveTo->setTag(moveCameraTag);
        m_camera->runAction(moveTo);
        
        m_world->updateCullingRect(Rect(viewPoint - (winSize * coefficientOffsetSize / 2.0f),
            winSize * (1.0f + coefficientOffsetSize)));

        this->m_gameLoop->step();
    };
    
    return true;
}

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
