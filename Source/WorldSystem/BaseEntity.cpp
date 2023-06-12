#include "WorldSystem/BaseEntity.h"

#include "WorldSystem/World.h"

void BaseEntity::setPosition(const cocos2d::Vec2& position)
{
    BaseEntity::setPosition(position.x, position.y);
}

void BaseEntity::setPosition(float x, float y)
{
    m_position = m_world->convertToCellSpace({x, y});
    Node::setPosition(x, y);
}

void BaseEntity::setScheduleMovePositionOnMap(Vec2Int position)
{
    moved(this, m_position, position);
}

void BaseEntity::setPositionOnMapWithoutNotify(Vec2Int cellPosition)
{
    setPosition(m_world->convertToMapSpace(cellPosition));
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
