#pragma once

#include <cstdint>

struct Vec2Int
{
    Vec2Int() = default;

    Vec2Int(const Vec2Int&) = default;
    Vec2Int(Vec2Int&&) = default;

    Vec2Int& operator=(const Vec2Int&) = default;
    Vec2Int& operator=(Vec2Int&&) = default;

    Vec2Int(int64_t _x, int64_t _y) : x(_x), y(_y) {}

    int64_t x = 0;
    int64_t y = 0;
};
