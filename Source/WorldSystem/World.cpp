#include "WorldSystem/World.h"

using namespace cocos2d;

World* World::create(TMXTiledMap* tilemap)
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

    const TMXLayer* background = m_tilemap->getLayer("Background");

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

Size World::getSize() const
{
    return m_tilemap->getMapSize();
}

Size World::getTileSize() const
{
    return m_tilemap->getTileSize();
}

World::World(TMXTiledMap* tilemap)
    : m_tilemap(tilemap) { }
