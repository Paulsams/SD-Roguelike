#include "WorldSystem/BaseEntity.h"

#include "WorldSystem/World.h"

void BaseEntity::setPositionInWorld(Vec2Int position)
{
    Vec2Int oldPosition = position;
    m_position = position;
    setPosition(m_world->convertToWorldSpace(position));
    moved(oldPosition, m_position);
}
