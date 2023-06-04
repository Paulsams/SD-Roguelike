#pragma once
#include "math/Vec2.h"

enum DirectionType
{
    Left,
    Up,
    Right,
    Down
};

class Direction
{
public:
    const DirectionType type;
    
    explicit Direction(DirectionType type)
        : type(type) { }

    cocos2d::Vec2 getVector() const
    {
        switch (type)
        {
        case Left:
            return {-1, 0};
        case Up:
            return {0, 1};
        case Right:
            return {1, 0};
        case Down:
            return {0, -1};
        }
    }
};
