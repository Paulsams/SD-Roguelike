#include "WorldSystem/World.h"
#include "Player/Player.h"

using namespace cocos2d;

World* World::create(Tilemap* tilemap)
{
    auto world = new (std::nothrow) World(tilemap);
    if (world && world->init())
    {
        world->autorelease();
        return world;
    }
    CC_SAFE_DELETE(world);
    return nullptr;
}

bool World::init()
{
    this->addChild(m_tilemap, 0, 99);

    m_background = m_tilemap->getLayer("Background");

    const TMXObjectGroup* objectGroup = m_tilemap->getObjectGroup("Objects");
    
    if(objectGroup == nullptr) {
        log("tile map has no objects object layer");
        return false;
    }

    const ValueMap spawnPoint = objectGroup->getObject("SpawnPoint");
    m_spawnPoint = Point(spawnPoint.at("x").asInt(),
                         spawnPoint.at("y").asInt());

    return true;
}

void World::addEntity(BaseEntity* entity)
{
    Vec2Int positionInWorld = entity->getPositionInWorld();
    size_t index = positionInWorld.x + positionInWorld.y * getSize().width;

    if (m_entities[index])
    {
        log("This cell is already occupied by the entity");
        return;
    }
    
    m_entities[index] = entity;
    entity->moved += m_movedEntityDelegate;
    entity->deleted += m_deletedEntityDelegate;
}

void World::removeEntity(BaseEntity* entity)
{
    const Vec2Int positionInWorld = entity->getPositionInWorld();
    const size_t index = positionInWorld.x + positionInWorld.y * getSize().width;
    entity->moved -= m_movedEntityDelegate;
    entity->deleted -= m_deletedEntityDelegate;
    m_entities[index] = nullptr;
}

bool World::tryGetEntity(Vec2Int position, const BaseEntity*& entity) const
{
    entity = m_entities[position.x + position.y * getSize().width];
    return static_cast<bool>(entity);
}

Size World::getSize() const
{
    return m_tilemap->getMapSize();
}

Size World::getTileSize() const
{
    return m_tilemap->getTileSize();
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

    return TileType::GROUND;
}

World::World(Tilemap* tilemap)
    : m_movedEntityDelegate(CC_CALLBACK_2(World::onEntityMoved, this))
    , m_deletedEntityDelegate(CC_CALLBACK_1(World::onDeletedEntity, this))
    , m_entities(std::make_shared<BaseEntity*[]>(tilemap->getMapSize().width * tilemap->getMapSize().height))
    , m_tilemap(tilemap)
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
