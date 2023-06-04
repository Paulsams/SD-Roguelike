#pragma once

#include "Utils/Common.h"

#include "2d/CCTMXTiledMap.h"

class RandomGeneratorWorldBuilder
{
public:
    RandomGeneratorWorldBuilder() = default;

    cocos2d::TMXTiledMap* build() const
    {
        if (m_height == 0 || m_width == 0 || m_iterCount == 0)
            return nullptr;

        return generateWorld();
    }

    RandomGeneratorWorldBuilder& setPath(const std::string& path)
    {
        m_path = path;
        return *this;
    }

    RandomGeneratorWorldBuilder& setHeight(size_t height)
    {
        m_height = height;
        return *this;
    }

    RandomGeneratorWorldBuilder& setWidth(size_t width)
    {
        m_width = width;
        return *this;
    }

    RandomGeneratorWorldBuilder& setIterCount(size_t iterCount)
    {
        m_iterCount = iterCount;
        return *this;
    }

    RandomGeneratorWorldBuilder& setDiscardByRatio(bool discardByRatio)
    {
        m_discardByRatio = discardByRatio;
        return *this;
    }

    RandomGeneratorWorldBuilder& setRoomVerticalRatio(float roomVerticalRatio)
    {
        m_roomVerticalRatio = roomVerticalRatio;
        return *this;
    }

    RandomGeneratorWorldBuilder& setRoomHorizontalRatio(float roomHorizontalRatio)
    {
        m_roomHorizontalRatio = roomHorizontalRatio;
        return *this;
    }

private:

    struct Container
    {
        Container(Vec2Int _point, int64_t _w, int64_t _h)
            : point(_point)
            , w(_w)
            , h(_h)
            , center(point.x + w/2, point.y + h/2)
        {

        }

        Vec2Int point;
        Vec2Int center;
        int64_t h = 0;
        int64_t w = 0;
    };

    struct Tree
    {

    };

    cocos2d::TMXTiledMap* generateWorld() const
    {
        cocos2d::TMXTiledMap* result = cocos2d::TMXTiledMap::create(m_path);



        return nullptr;
    }


    std::string m_path;

    size_t m_height = 0;
    size_t m_width = 0;
    size_t m_iterCount = 0;
    bool m_discardByRatio = true;

    float m_roomVerticalRatio = 0.5;
    float m_roomHorizontalRatio = 0.5;
};