#include "WorldSystem/BaseEntity.h"

#include "WorldSystem/World.h"

void BaseEntity::setScheduleMovePositionOnMap(Vec2Int position)
{
    m_world->scheduleMove(this, m_position, position);
    moved(this, m_position, position);
}

void BaseEntity::setPositionOnMapWithoutNotify(Vec2Int cellPosition)
{
    m_position = cellPosition;
    Node::setPosition(m_world->convertToMapSpace(m_position));
}

void BaseEntity::moveOnMapTo(Vec2Int endPosition, float time)
{
    static constexpr int moveTag = 10;

    stopActionByTag(moveTag);
    
    const auto moveTo = cocos2d::MoveTo::create(time, m_world->convertToMapSpace(endPosition));
    moveTo->setTag(moveTag);
    Node::runAction(moveTo);
    m_position = endPosition;
}

void BaseEntity::destroyEntity()
{
    deleted(this);
    m_world->removeEntity(this);
}
