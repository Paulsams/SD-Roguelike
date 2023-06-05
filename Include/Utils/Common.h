#pragma once

#include <cstdint>

#include "2d/CCFastTMXTiledMap.h"
#include "2d/CCTMXTiledMap.h"
#include "math/Vec2.h"

using Tilemap = cocos2d::FastTMXTiledMap;
using TilemapLayer = cocos2d::FastTMXLayer;

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

    inline Vec2Int operator+(const Vec2Int& v) const
    {
        Vec2Int result(*this);
        result.add(v);
        return result;
    }

    inline Vec2Int& operator+=(const Vec2Int& v)
    {
        add(v);
        return *this;
    }

    inline Vec2Int operator-(const Vec2Int& v) const
    {
        Vec2Int result(*this);
        result.subtract(v);
        return result;
    }

    inline Vec2Int& operator-=(const Vec2Int& v)
    {
        subtract(v);
        return *this;
    }

    inline Vec2Int operator-() const
    {
        Vec2Int result(*this);
        result.negate();
        return result;
    }

    inline Vec2Int operator*(float s) const
    {
        Vec2Int result(*this);
        result.scale(s);
        return result;
    }

    inline Vec2Int& operator*=(float s)
    {
        scale(s);
        return *this;
    }

    inline Vec2Int operator/(const float s) const
    {
        return Vec2Int(this->x / s, this->y / s);
    }

    inline void add(const Vec2Int& v)
    {
        x += v.x;
        y += v.y;
    }

    inline void negate()
    {
        x = -x;
        y = -y;
    }

    inline void scale(float scalar)
    {
        x *= scalar;
        y *= scalar;
    }

    inline void subtract(const Vec2Int& v)
    {
        x -= v.x;
        y -= v.y;
    }

    Vec2Int(int64_t _x, int64_t _y) : x(_x), y(_y) {}

    int64_t x = 0;
    int64_t y = 0;
};
