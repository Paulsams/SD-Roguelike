#pragma once

#include <cstdint>

#include "2d/CCTMXTiledMap.h"
#include "math/Vec2.h"

using Tilemap = cocos2d::TMXTiledMap;
using TilemapLayer = cocos2d::TMXLayer;

template<class T, class U>
concept Derived = std::is_base_of_v<U, T>;

struct Paths
{
    inline static const std::string toUITileset = "Tileset.png";
    inline static const std::string toGameTileset = "Tileset.png";
    inline static const std::string toIconFrame = "IconFrame.png";
};

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
    
    Vec2Int(int64_t _x, int64_t _y) : x(_x), y(_y) { }

    operator cocos2d::Vec2() const { return cocos2d::Vec2{static_cast<float>(x), static_cast<float>(y)}; }

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

    inline float distance(const Vec2Int& v) const
    {
        float dx = v.x - x;
        float dy = v.y - y;

        return std::sqrt(dx * dx + dy * dy);
    }

    inline float distanceSquared(const Vec2Int& v) const
    {
        float dx = v.x - x;
        float dy = v.y - y;
        return (dx * dx + dy * dy);
    }

    friend bool operator<(const Vec2Int& lhs, const Vec2Int& rhs)
    {
        if (lhs.x < rhs.x)
            return true;
        if (rhs.x < lhs.x)
            return false;
        return lhs.y < rhs.y;
    }

    friend bool operator<=(const Vec2Int& lhs, const Vec2Int& rhs)
    {
        return rhs >= lhs;
    }

    friend bool operator>(const Vec2Int& lhs, const Vec2Int& rhs)
    {
        return rhs < lhs;
    }

    friend bool operator>=(const Vec2Int& lhs, const Vec2Int& rhs)
    {
        return !(lhs < rhs);
    }

    friend bool operator==(const Vec2Int& lhs, const Vec2Int& rhs)
    {
        return lhs.x == rhs.x
            && lhs.y == rhs.y;
    }

    friend bool operator!=(const Vec2Int& lhs, const Vec2Int& rhs)
    {
        return !(lhs == rhs);
    }

    static std::size_t hash_value(const Vec2Int& obj)
    {
        std::size_t seed = 0x02BEF05A;
        seed ^= (seed << 6) + (seed >> 2) + 0x1DBA603F + static_cast<std::size_t>(obj.x);
        seed ^= (seed << 6) + (seed >> 2) + 0x224CB762 + static_cast<std::size_t>(obj.y);
        return seed;
    }

    int64_t x = 0;
    int64_t y = 0;
};
