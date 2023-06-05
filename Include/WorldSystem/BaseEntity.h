#pragma once

#include "Utils/Common.h"
#include "Utils/EventsSystem.h"

class BaseEntity
{
public:
    using oldPosition = Vec2Int;
    using newPosition = Vec2Int;
    
    EventContainer<oldPosition, newPosition> moved;
    EventContainer<> deleted;

    Vec2Int getPositionInWorld() const { return m_position; }

    void setPositionInWorld(Vec2Int position)
    {
        Vec2Int oldPosition = position; 
        m_position = position;
        moved(oldPosition, m_position);
    }
    
private:
    Vec2Int m_position;
};
