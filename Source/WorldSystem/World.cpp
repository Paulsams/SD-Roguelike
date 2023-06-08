#include "WorldSystem/World.h"

#include "Pathfinder/PathfinderAStar.h"
#include "Player/Player.h"
#include "WorldSystem/Decoration.h"
#include "WorldSystem/WorldTileConfig.h"

using namespace cocos2d;

World* World::create(Tilemap* tilemap, const TilemapConfig& levelConfig)
{
    auto world = new (std::nothrow) World(tilemap);
    if (world && world->initWithConfig(levelConfig))
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

bool World::initWithConfig(const TilemapConfig& levelConfig)
{
    this->addChild(m_tilemap, 0, 99);

    m_background = m_tilemap->getLayer("Background");
    m_graph = std::make_shared<pathfinder::Graph>(m_tilemap->getMapSize(), m_background->getTiles(),
        [levelConfig](uint32_t gid) { return levelConfig.getTileGround(gid); });

    const TMXObjectGroup* objectsGroup = m_tilemap->getObjectGroup("Objects");
    const TMXObjectGroup* enemiesGroup = m_tilemap->getObjectGroup("Enemies");
    const TMXObjectGroup* chestsGroup = m_tilemap->getObjectGroup("Chests");
    const TMXObjectGroup* decorationsGroup = m_tilemap->getObjectGroup("Decorations");

    if(!objectsGroup || !enemiesGroup || !chestsGroup || !decorationsGroup) {
        log("Tile map has no needed objects layer");
        return false;
    }

    Size tileSize = getTileSize();
    const ValueMap spawnPoint = objectsGroup->getObject("SpawnPoint");
    m_spawnPoint = readPositionFromRectangle(spawnPoint, tileSize);

    for (const auto& decorationObject : decorationsGroup->getObjects())
    {
        const ValueMap& decorationMap = decorationObject.asValueMap();
        int gid = decorationMap.at("gid").asInt();
        auto decoration = Decoration::create(this, getRectFromGid(gid - 1));
        Vec2Int position = readPositionFromTile(decorationMap, tileSize);
        decoration->setPositionOnMap(readPositionFromTile(decorationMap, tileSize));
        addEntity(decoration);
        m_graph->getNodeByPos(position)->tile = TileType::OBSTACLE;
    }

    return true;
}

void World::addEntity(BaseEntity* entity)
{
    Vec2Int positionInWorld = entity->getPositionOnMap();
    size_t index = getIndexFromVec2(positionInWorld);
    
    m_entities[index].push_back(entity);
    entity->moved += m_movedEntityDelegate;
    entity->deleted += m_deletedEntityDelegate;
    this->addChild(entity);
}

void World::removeEntity(BaseEntity* entity)
{
    const Vec2Int positionInWorld = entity->getPositionOnMap();
    entity->moved -= m_movedEntityDelegate;
    entity->deleted -= m_deletedEntityDelegate;
    std::vector<BaseEntity*> entitiesOnCurrentCell = m_entities[getIndexFromVec2(positionInWorld)];
    entitiesOnCurrentCell.erase(std::find(entitiesOnCurrentCell.begin(), entitiesOnCurrentCell.end(), entity));
}

void World::addPlayer(Player* player)
{
    m_player = player;
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

World::World(Tilemap* tilemap)
    : m_movedEntityDelegate(CC_CALLBACK_2(World::onEntityMoved, this))
    , m_deletedEntityDelegate(CC_CALLBACK_1(World::onDeletedEntity, this))
    , m_entities(std::vector<std::vector<BaseEntity*>>
        (tilemap->getMapSize().width * tilemap->getMapSize().height))
    , m_tilemap(tilemap)
    , m_pathfinding(std::make_shared<pathfinder::PathfinderAStar>())
{
    Node::setContentSize({m_tilemap->getMapSize().width * m_tilemap->getTileSize().width,
        m_tilemap->getMapSize().height * m_tilemap->getTileSize().height});
}

void World::onEntityMoved(BaseEntity::oldPosition oldPosition, BaseEntity::newPosition newPosition)
{
    
}

void World::onDeletedEntity(BaseEntity* entity)
{
    removeEntity(entity);
}
