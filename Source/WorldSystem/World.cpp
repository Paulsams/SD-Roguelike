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

Point World::getSpawnPoint() const
{
    return m_spawnPoint;
}

// bool World::tryGetEntity(Vec2Int position, std::shared_ptr<const BaseEntity>& entity)
// {
//     //entity = m_entities[position.x + position.y * m_tilemap->getMapSize()]
// }

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

World::World(Tilemap* tilemap)
    : m_entities(tilemap->getMapSize().width * tilemap->getMapSize().height)
    , m_tilemap(tilemap)
{
    Node::setContentSize({m_tilemap->getMapSize().width * m_tilemap->getTileSize().width,
        m_tilemap->getMapSize().height * m_tilemap->getTileSize().height});
}
