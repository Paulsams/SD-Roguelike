#include "WorldSystem/World.h"

#include "ItemsSystem/Items.h"
#include "Pathfinder/PathfinderAStar.h"
#include "Player/Player.h"
#include "WorldSystem/Chest.h"
#include "WorldSystem/Decoration.h"
#include "WorldSystem/WorldTileConfig.h"

using namespace cocos2d;

World* World::create(Tilemap* tilemap, std::shared_ptr<mob::BaseMobAbstractFactory> mobFactory)
{
    auto world = new (std::nothrow) World(tilemap, mobFactory);
    if (world && world->initWithTilemap())
    {
        world->autorelease();
        return world;
    }
    CC_SAFE_DELETE(world);
    return nullptr;
}

Vec2Int readPositionFromRectangle(const ValueMap& valueMap, Size tileSize)
{
    return Vec2Int(valueMap.at("x").asInt() / static_cast<int>(tileSize.width),
                   valueMap.at("y").asInt() / static_cast<int>(tileSize.height));
}

Vec2Int readPositionFromTile(const ValueMap& valueMap, Size tileSize)
{
    Vec2Int pos = readPositionFromRectangle(valueMap, tileSize);
    pos.y += 1;
    return pos;
}

bool World::initWithTilemap()
{
    this->addChild(m_tilemap, 0, 99);

    m_ground = m_tilemap->getLayer("Ground");
    m_walls = m_tilemap->getLayer("Walls");
    m_graph = std::make_shared<pathfinder::Graph>(m_tilemap->getMapSize(), m_walls->getTiles(),
        [](uint32_t gid) { return gid == 0 ? TileType::GROUND : TileType::OBSTACLE; });

    const TMXObjectGroup* objectsGroup = m_tilemap->getObjectGroup("UtilsObjects");
    const TMXObjectGroup* normalMobsGroup = m_tilemap->getObjectGroup("NormalMobs");
    const TMXObjectGroup* eliteMobsGroup = m_tilemap->getObjectGroup("EliteMobs");
    const TMXObjectGroup* bossMobsGroup = m_tilemap->getObjectGroup("BossMobs");
    const TMXObjectGroup* passiveMobsGroup = m_tilemap->getObjectGroup("PassiveMobs");
    const TMXObjectGroup* chestsGroup = m_tilemap->getObjectGroup("Chests");
    const TMXObjectGroup* decorationsGroup = m_tilemap->getObjectGroup("Decorations");
    
    if (!objectsGroup)
    {
        log("Tile map has no needed objects layer");
        return false;
    }

    const Size tileSize = getTileSize();
    const ValueMap spawnPoint = objectsGroup->getObject("SpawnPoint");
    m_spawnPoint = readPositionFromRectangle(spawnPoint, tileSize);

    tryInitDecorations(decorationsGroup, tileSize);
    tryInitChests(chestsGroup, tileSize);
    
    trySpawnMobs(normalMobsGroup, tileSize, &mob::BaseMobAbstractFactory::createNormal);
    trySpawnMobs(eliteMobsGroup, tileSize, &mob::BaseMobAbstractFactory::createElite);
    trySpawnMobs(bossMobsGroup, tileSize, [this](mob::BaseMobAbstractFactory* factory, World* world, int gid)
    {
        mob::Mob* boss = factory->createBoss(world, gid);
        m_bosses.push_back(boss);
        return boss;
    });
    trySpawnMobs(passiveMobsGroup, tileSize, &mob::BaseMobAbstractFactory::createPassive);

    m_damageIndicatorsMobs = DamageIndicatorsSystems::create(this);
    this->addChild(m_damageIndicatorsMobs);
 
    return true;
}

void World::tryInitDecorations(const TMXObjectGroup* decorationsGroup, const Size tileSize)
{
    if (!decorationsGroup)
        return;
    
    for (const auto& decorationObject : decorationsGroup->getObjects())
    {
        const ValueMap& decorationMap = decorationObject.asValueMap();
        const int gid = decorationMap.at("gid").asInt();
        
        const auto decoration = Decoration::create(this, getRectFromGid(gid - 1));
        const Vec2Int position = readPositionFromTile(decorationMap, tileSize);
        decoration->setPositionOnMapWithoutNotify(position);
        addEntity(decoration);
        m_graph->getNodeByPos(position)->tile = TileType::DECORATION;

        const auto nameLootIterator = decorationMap.find("loot");
        if (nameLootIterator != decorationMap.end())
        {
            std::string nameLoot = nameLootIterator->second.asString();
            decoration->deleted += [this, nameLoot](BaseEntity* entity)
            {
                spawnItem(entity->getPositionOnMap(), [this, nameLoot]()
                {
                    return Items::createConsumable(this, nameLoot);
                });
            };
        }
    }
}

void World::tryInitChests(const TMXObjectGroup* chestsGroup, const Size tileSize)
{
    if (!chestsGroup)
        return;

    for (const auto& chestObject : chestsGroup->getObjects())
    {
        const ValueMap& chestMap = chestObject.asValueMap();
        const int gid = chestMap.at("gid").asInt();
        
        const auto chest = Chest::create(this, {Paths::toGameTileset, getRectFromGid(gid - 1)},
            [this, chestMap](Chest* chest)
            {
                const auto nameLootIterator = chestMap.find("loot");
                if (nameLootIterator != chestMap.end())
                {
                    const std::string weaponInfo = nameLootIterator->second.asString();
                    const size_t weaponSplitIndex = nameLootIterator->second.asString().find(':');
                    std::string nameWeapon = weaponInfo.substr(0, weaponSplitIndex);
                    int tier = std::stoi(weaponInfo.substr(weaponSplitIndex + 1));
                    // static std::vector<Vec2Int> directionsSpawn =
                    //     {
                    //         {0, -1},
                    //         {1, 0},
                    //         {-1, 0},
                    //         {0, 1}
                    //     };
                    //
                    // std::function<BaseItem*()> spawnItemFunc = [this, nameWeapon, tier]()
                    //     {
                    //        return Attacks::createWeapon(this, nameWeapon, tier);
                    //     };
                    // for (const auto direction : directionsSpawn)
                    // {
                    //     if (spawnItem(chest, direction, spawnItemFunc))
                    //         break;
                    // }
                    World* world = this;
                    Vec2Int chestPosition = chest->getPositionOnMap();
                    chest->destroyEntity();
                    world->spawnItem(chestPosition, [world, nameWeapon, tier]()
                    {
                        return Attacks::createWeapon(world, nameWeapon, tier);
                    });
                }
            });
        const Vec2Int position = readPositionFromTile(chestMap, tileSize);
        chest->setPositionOnMapWithoutNotify(position);
        addEntity(chest);
        m_graph->getNodeByPos(position)->tile = TileType::OBSTACLE;
    }
}

bool World::spawnItem(Vec2Int position, const std::function<BaseItem*()>& createFunc)
{
    // const TileType tileType = getTileType(position);
    // if (tileType == TileType::GROUND)
    // {
    BaseItem* item = createFunc();
    item->setPositionOnMapWithoutNotify(position);
    addEntity(item);
    return true;
    // }
    //
    // return false;
}

void World::trySpawnMobs(const TMXObjectGroup* group, const Size tileSize,
    const std::function<mob::Mob*(mob::BaseMobAbstractFactory*, World*, int)>& createFunc)
{
    if (group == nullptr)
        return;
    
    for (const auto& mobObject : group->getObjects())
    {
        const ValueMap& mobMap = mobObject.asValueMap();
        const int gid = mobMap.at("gid").asInt();
        const auto mob = createFunc(m_mobFactory.get(), this, gid);
        const Vec2Int position = readPositionFromTile(mobMap, tileSize);
        mob->setPositionOnMapWithoutNotify(position);
        addEntity(mob);
    }
}

void World::addEntity(BaseEntity* entity)
{
    static std::shared_ptr<FunctionVisitorEntities<int>> visitorGetZOrder = FunctionVisitorEntitiesBuilder<int>()
        .setChest([](Chest*)           { return 4; })
        .setDecoration([](Decoration*) { return 4; })
        .setItem([](BaseItem*)         { return 1; })
        .setMob([](mob::Mob*)          { return 5; })
        .setPlayer([](Player*)         { return 6; })
        .build();
    
    const Vec2Int positionOnMap = entity->getPositionOnMap();
    const size_t index = getIndexFromVec2(positionOnMap);
    
    m_entities[index].push_back(entity);
    entity->acceptVisit(visitorGetZOrder);
    this->addChild(entity, visitorGetZOrder->getReturnValue().value());

    entity->acceptVisit(m_visitorFromAddEntity);
}

void World::removeEntity(BaseEntity* entity)
{
    const Vec2Int positionOnMap = entity->getPositionOnMap();
    std::vector<BaseEntity*>& entitiesOnCurrentCell = m_entities[getIndexFromVec2(positionOnMap)];
    entitiesOnCurrentCell.erase(std::ranges::find(entitiesOnCurrentCell, entity));
    updateTileType(positionOnMap);
    entity->acceptVisit(m_visitorFromRemoveEntity);
    m_movedEntities.erase(entity);

    auto finded = std::ranges::find(m_bosses, entity);
    if (finded != m_bosses.end())
    {
        m_bosses.erase(finded);
        bossCountChanged(m_bosses);
    }
    
    entity->removeFromParent();
}

void World::scheduleMove(BaseEntity* entity, BaseEntity::oldPosition oldPosition, BaseEntity::newPosition newPosition)
{
    const auto finded = m_movedEntities.find(entity);
    if (finded != m_movedEntities.end())
        m_movedEntities.at(entity).second = newPosition;
    else
        m_movedEntities.insert({entity, {oldPosition, newPosition}});
}

void World::addPlayer(Player* player)
{
    m_player = player;
    addEntity(m_player);

    DamageIndicatorsSystems* newIndicators = DamageIndicatorsSystems::create(this);
    this->addChild(newIndicators);
    m_playersDamageIndicators.insert({player, newIndicators});
}

void World::removePlayer(Player* player)
{
    player->throwAllItems();
    removeEntity(player);

    DamageIndicatorsSystems* newIndicators = m_playersDamageIndicators.at(player);
    m_playersDamageIndicators.erase(player);
    newIndicators->removeFromParent();
    
    m_player = nullptr;
}

const Player* World::getNearestPlayer(Vec2) const
{
    return m_player;
}

TileType World::getTileType(Vec2Int position) const
{
    const Size size = getSize();
    if (position.x < 0 || position.x >= size.width || position.y < 0 || position.y >= size.height)
        return TileType::OUT_OF_BOUNDS;

    return m_graph->getNodeByPos(position)->tile;
}

std::vector<Vec2Int> World::findPath(Vec2Int start, Vec2Int finish) const
{
    return m_pathfinding->findPath(*m_graph, m_graph->getNodeByPos(start),
        m_graph->getNodeByPos(finish),
        [this](const pathfinder::Node* node)
        {
            for (BaseEntity* entity : getEntitiesFromCell(node->pos))
            {
                entity->acceptVisit(m_visitorFindPath);
                if (m_visitorFindPath->getReturnValue().value_or(false))
                    return false;
            }
            return true;
        });
}

void World::update()
{
    if (m_player)
        m_player->update();

    checkMoveEntities();
    m_movedEntities.clear();
    
    for (mob::Mob* mob : m_mobs)
        mob->update();
    
    checkMoveEntities();
    m_movedEntities.clear();
    
    m_currentMovedEntities.clear();

    for (auto [player, damageIndicators] : m_playersDamageIndicators)
        damageIndicators->update();
    m_damageIndicatorsMobs->update();
}

void World::checkMoveEntities()
{
    for (const auto& [mob, positions] : m_movedEntities)
    {
        auto [oldPosition, newPosition] = positions;

        if (m_currentMovedEntities.emplace(newPosition).second)
        {
            static constexpr float moveTime = 0.1f;
            
            std::vector<BaseEntity*>& oldEntities = m_entities[getIndexFromVec2(oldPosition)];
            oldEntities.erase(std::ranges::find(oldEntities, mob));
            m_entities[getIndexFromVec2(newPosition)].push_back(mob);
            mob->moveOnMapTo(newPosition, moveTime);
        }
    }
}

World::World(Tilemap* tilemap, std::shared_ptr<mob::BaseMobAbstractFactory> mobFactory)
    : m_mobFactory(mobFactory)
    , m_entities(std::vector<std::vector<BaseEntity*>>
        (tilemap->getMapSize().width * tilemap->getMapSize().height))
    , m_tilemap(tilemap)
    , m_pathfinding(std::make_shared<pathfinder::PathfinderAStar>())
{
    Node::setContentSize({m_tilemap->getMapSize().width * m_tilemap->getTileSize().width,
        m_tilemap->getMapSize().height * m_tilemap->getTileSize().height});

    m_visitorFromAddEntity = FunctionVisitorEntitiesBuilder<void>()
        .setMob([this](mob::Mob* mob)
        {
            m_mobs.push_front(mob);
        }).build();

    m_visitorFromRemoveEntity = FunctionVisitorEntitiesBuilder<void>()
        .setMob([this](mob::Mob* mob) { m_mobs.erase(std::ranges::find(m_mobs, mob)); })
        .build();

    m_visitorFindPath = FunctionVisitorEntitiesBuilder<bool>()
        .setMob([this](mob::Mob*) { return true; })
        .setPlayer([this](Player*) { return true; })
        .build();
}

void World::updateTileType(Vec2Int position) const
{
    static std::shared_ptr<FunctionVisitorEntities<TileType>> visitor = FunctionVisitorEntitiesBuilder<TileType>()
        .setDecoration([](Decoration*){ return TileType::DECORATION; })
        .build();
    
    const std::vector<BaseEntity*>& entities = m_entities[getIndexFromVec2(position)];
    TileType tileType = TileType::GROUND;
    for (BaseEntity* entity : entities)
    {
        entity->acceptVisit(visitor);
        std::optional<TileType> returnTileType = visitor->getReturnValue();
        if (returnTileType.has_value())
        {
            tileType = returnTileType.value();
            break;
        }
    }
    
    m_graph->getNodeByPos(position)->tile = tileType;
}
