#pragma once

#include "Stats/IHaveStats.h"
#include "Utils/Common.h"
#include "Utils/EventsSystem.h"

struct IVisitorEntities;
class World;

class BaseEntity : public virtual cocos2d::Node, public IHaveStats
{
public:
    using oldPosition = Vec2Int;
    using newPosition = Vec2Int;

    Vec2Int getPositionOnMap() const { return m_position; }
    World* getWorld() const { return m_world; }
    
    void setPositionOnMap(Vec2Int position);
    
    EventContainer<oldPosition, newPosition> moved;
    EventContainer<BaseEntity*> deleted;

    virtual void acceptVisit(std::shared_ptr<IVisitorEntities> visitor) = 0;

protected:
    explicit BaseEntity(World* world)
        : m_world(world) { }

    void destroyEntity()
    {
        deleted(this);
        getParent()->removeChild(this);
    }
    
private:
    Vec2Int m_position;
    World* m_world;
};
