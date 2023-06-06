#pragma once

#include "Stats/IHaveStats.h"
#include "Utils/Common.h"
#include "Utils/EventsSystem.h"

class World;

class BaseEntity : public cocos2d::Node, public IHaveStats
{
public:
    using oldPosition = Vec2Int;
    using newPosition = Vec2Int;

    BaseEntity(World* world)
        : m_world(world) { }

    Vec2Int getPositionInWorld() const { return m_position; }
    World* getWorld() const { return m_world; }
    
    void setPositionInWorld(Vec2Int position);
    
    EventContainer<oldPosition, newPosition> moved;
    EventContainer<BaseEntity*> deleted;
    
private:
    Vec2Int m_position;
    World* m_world;
};
