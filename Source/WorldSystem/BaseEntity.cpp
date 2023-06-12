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

void BaseEntity::setMovedPositionOnMap(Vec2Int position)
{
    moved(this, m_position, position);
}

void BaseEntity::setPositionOnMapWithoutNotify(Vec2Int cellPosition)
{
    setPosition(m_world->convertToMapSpace(cellPosition));
}
