#pragma once

#include "Stats/IHaveStats.h"
#include "Utils/Common.h"
#include "Utils/EventsSystem.h"

struct IVisitorEntities;
class World;

/**
 * An entity that is associated with the world and is a representative of an element in it
*/
class BaseEntity : public virtual cocos2d::Node, public IHaveStats
{
public:
    using OldPosition = Vec2Int;
    using NewPosition = Vec2Int;

    /**
     * @return position relative to the card that belongs to the World
     */
    Vec2Int getPositionOnMap() const { return m_position; }
    /**
     * @return World to which this entity belongs at the moment
     */
    World* getWorld() const { return m_world; }
    
    /**
    * Schedule move after all updates of other entities in this frame
     * @param position position relative to the map
     */
    void setScheduleMovePositionOnMap(Vec2Int position);
    /**
    * Set position on the map immediately and without event notification
     * @param cellPosition position relative to the map
     */
    void setPositionOnMapWithoutNotify(Vec2Int cellPosition);
    /**
     * Create an action to smoothly move to the specified position and run it
     * @param endPosition position relative to the map
     * @param time time it takes to move
     */
    void moveOnMapTo(Vec2Int endPosition, float time);

    /**
     * Notifies of deletion and then removes the entity from the world
     */
    void destroyEntity();
    
    /**
     * An event called when the player moves, which spawns the world after all entity updates
     */
    EventContainer<BaseEntity*, OldPosition, NewPosition> moved;
    /**
     * Event called before deleting an entity
     */
    EventContainer<BaseEntity*> deleted;

    /**
     * Accept visitor with given function
     * @param visitor visitor
     */
    virtual void acceptVisit(std::shared_ptr<IVisitorEntities> visitor) = 0;

protected:
    explicit BaseEntity(World* world)
        : m_world(world) { }

private:
    Vec2Int m_position;
    World* m_world;
};
