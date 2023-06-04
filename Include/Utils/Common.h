#pragma once

#include <cstdint>

#include "math/Vec2.h"

struct Vec2Int
{
    Vec2Int() = default;

    Vec2Int(const Vec2Int&) = default;
    Vec2Int(Vec2Int&&) = default;

    Vec2Int& operator=(const Vec2Int&) = default;
    Vec2Int& operator=(Vec2Int&&) = default;

    explicit Vec2Int(const cocos2d::Vec2& other) : x(other.x), y(other.y) {}
    Vec2Int& operator=(const cocos2d::Vec2& other)
    {
        x = other.x;
        y = other.y;
        return *this;
    }

    Vec2Int(int64_t _x, int64_t _y) : x(_x), y(_y) {}

    int64_t x = 0;
    int64_t y = 0;
};
