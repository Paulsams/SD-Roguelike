#pragma once

#include "Common.h"

enum DirectionType
{
    LEFT,
    UP,
    RIGHT,
    DOWN
};


/**
 * 4-way direction
 */
struct Direction
{
    const DirectionType type;
    
    Direction(DirectionType type)
        : type(type)
    {}

    Direction(const cocos2d::Vec2 vector)
        : type(getDirectionFromVector(vector))
    {}

    /**
     * @return DirectionType from math point
     */
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
        default:
            return {0, 0};
        }
    }

    /**
     * Rotate given vector with current direction
     * @param vector vector
     * @return Rotated vector
     */
    Vec2Int rotate(Vec2Int vector) const
    {
        switch (type)
        {
        case LEFT:
            return -vector;
        case UP:
            return {-vector.y, vector.x};
        case RIGHT:
            return vector;
        case DOWN:
            return {vector.y, -vector.x};
        default:
            return {0, 0};
        }
    }

    friend bool operator==(const Direction& lhs, const Direction& rhs)
    {
        return lhs.type == rhs.type;
    }

    friend bool operator!=(const Direction& lhs, const Direction& rhs)
    {
        return !(lhs == rhs);
    }

private:
    static DirectionType getDirectionFromVector(cocos2d::Vec2 vector)
    {
        vector.normalize();
        if (std::abs(vector.x) > std::abs(vector.y))
            return vector.x >= 0 ? RIGHT : LEFT;
        else
            return vector.y >= 0 ? UP : DOWN;
    }
};
