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
    trySpawnMobs(bossMobsGroup, tileSize, &mob::BaseMobAbstractFactory::createBoss);
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
                spawnItem(entity, {0, 0}, [this, nameLoot]()
                {
                    return Items::createConsumable(this, nameLoot);
                });
            };
        }
    }
}

void World::tryInitChests(const TMXObjectGroup* chestsGroup, const Size tileSize)
{
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
                    static std::vector<Vec2Int> directionsSpawn =
                        {
                            {0, -1},
                            {1, 0},
                            {-1, 0},
                            {0, 1}
                        };

                    std::function<BaseItem*()> spawnItemFunc = [this, nameWeapon, tier]()
                        {
                           return Attacks::createWeapon(this, nameWeapon, tier);
                        };
                    for (const auto direction : directionsSpawn)
                    {
                        if (spawnItem(chest, direction, spawnItemFunc))
                            break;
                    }
                }
            });
        const Vec2Int position = readPositionFromTile(chestMap, tileSize);
        chest->setPositionOnMapWithoutNotify(position);
        addEntity(chest);
        m_graph->getNodeByPos(position)->tile = TileType::OBSTACLE;
    }
}

bool World::spawnItem(BaseEntity* entity, Vec2Int direction, const std::function<BaseItem*()>& createFunc)
{
    const Vec2Int position = entity->getPositionOnMap() + direction;
    const TileType tileType = getTileType(position);
    if (tileType == TileType::GROUND)
    {
        BaseItem* item = createFunc();
        item->setPositionOnMapWithoutNotify(position);
        addEntity(item);
        return true;
    }
    
    return false;
}

void World::trySpawnMobs(const TMXObjectGroup* group, const Size tileSize,
    std::function<mob::Mob*(mob::BaseMobAbstractFactory*, World*, int)> createFunc)
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
    static std::shared_ptr<FunctionVisitorEntitiesReturnVoid> visitor = FunctionVisitorEntitiesBuilder<void>()
        .setMob([this](mob::Mob* mob) { m_mobs.push_back(mob); }).build();

    const Vec2Int positionOnMap = entity->getPositionOnMap();
    const size_t index = getIndexFromVec2(positionOnMap);
    
    m_entities[index].push_back(entity);
    entity->moved += m_movedEntityDelegate;
    entity->deleted += m_deletedEntityDelegate;
    this->addChild(entity, 10);

    entity->acceptVisit(visitor);
}

void World::removeEntity(BaseEntity* entity)
{
    internalRemoveEntity(entity);
    entity->moved -= m_movedEntityDelegate;
    entity->deleted -= m_deletedEntityDelegate;
}

void World::addPlayer(Player* player)
{
    m_player = player;
    addEntity(m_player);

    DamageIndicatorsSystems* newIndicators = DamageIndicatorsSystems::create(this);
    this->addChild(newIndicators);
    m_playersDamageIndicators.insert({player, newIndicators});
}

const Player* World::getNearestPlayer(Vec2) const
{
    return m_player;
}

TileType World::getTileType(Vec2Int position) const
{
    Size size = getSize();
    if (position.x < 0 || position.x >= size.width || position.y < 0 || position.y >= size.height)
        return TileType::OUT_OF_BOUNDS;

    return m_graph->getNodeByPos(position)->tile;
}

void World::update()
{
    m_movedEntities.clear();
    
    m_player->update();
    for (mob::Mob* mob : m_mobs)
        mob->update();

    for (auto [player, damageIndicators] : m_playersDamageIndicators)
        damageIndicators->update();
    m_damageIndicatorsMobs->update();
}

World::World(Tilemap* tilemap, std::shared_ptr<mob::BaseMobAbstractFactory> mobFactory)
    : m_movedEntityDelegate(CC_CALLBACK_3(World::onEntityMoved, this))
    , m_deletedEntityDelegate(CC_CALLBACK_1(World::onDeletedEntity, this))
    , m_mobFactory(mobFactory)
    , m_entities(std::vector<std::vector<BaseEntity*>>
        (tilemap->getMapSize().width * tilemap->getMapSize().height))
    , m_tilemap(tilemap)
    , m_pathfinding(std::make_shared<pathfinder::PathfinderAStar>())
{
    Node::setContentSize({m_tilemap->getMapSize().width * m_tilemap->getTileSize().width,
        m_tilemap->getMapSize().height * m_tilemap->getTileSize().height});
}

void World::updateTileType(Vec2Int position) const
{
    static std::shared_ptr<FunctionVisitorEntities<TileType>> visitor = FunctionVisitorEntitiesBuilder<TileType>().
        setDecoration([](Decoration*){ return TileType::DECORATION; }).build();
    
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

void World::onEntityMoved(BaseEntity* entity, BaseEntity::oldPosition oldPosition, BaseEntity::newPosition newPosition)
{
    static float moveTime = 0.1f;
    
    if (!m_movedEntities.insert({newPosition, entity}).second)
        return;

    std::vector<BaseEntity*>& oldEntities = m_entities[getIndexFromVec2(oldPosition)];
    oldEntities.erase(std::ranges::find(oldEntities, entity));
    m_entities[getIndexFromVec2(newPosition)].push_back(entity);
    entity->moveOnMapTo(newPosition, moveTime);
}

void World::internalRemoveEntity(BaseEntity* entity)
{
    std::shared_ptr<FunctionVisitorEntitiesReturnVoid> visitor = FunctionVisitorEntitiesBuilder<void>()
        .setMob([this](mob::Mob* mob) { m_mobs.erase(std::ranges::find(m_mobs, mob)); })
        .build();

    const Vec2Int positionOnMap = entity->getPositionOnMap();
    std::vector<BaseEntity*>& entitiesOnCurrentCell = m_entities[getIndexFromVec2(positionOnMap)];
    entitiesOnCurrentCell.erase(std::ranges::find(entitiesOnCurrentCell, entity));
    updateTileType(positionOnMap);
    entity->acceptVisit(visitor);
}

void World::onDeletedEntity(BaseEntity* entity)
{
    internalRemoveEntity(entity);
}
