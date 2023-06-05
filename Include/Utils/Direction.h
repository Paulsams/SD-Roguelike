#pragma once

#include "Common.h"

enum DirectionType
{
    LEFT,
    UP,
    RIGHT,
    DOWN
};

class Direction
{
public:
    const DirectionType type;
    
    explicit Direction(DirectionType type)
        : type(type) { }

    Vec2Int getVector() const
    {
        switch (type)
        {
        case LEFT:
            return {-1, 0};
        case UP:
            return {0, 1};
        case RIGHT:
            return {1, 0};
        case DOWN:
            return {0, -1};
        }
    }
};
