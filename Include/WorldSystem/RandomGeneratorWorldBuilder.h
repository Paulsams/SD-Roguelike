#pragma once

#include "Utils/Common.h"

#include "2d/CCFastTMXTiledMap.h"
#include "2d/CCTMXTiledMap.h"
#include "2d/CCFastTMXLayer.h"
#include "2d/CCTMXLayer.h"

#include <iostream>
#include <thread>
#include <cstring>

class RandomGeneratorWorldBuilder
{
public:
    RandomGeneratorWorldBuilder() = default;

    Tilemap* build() const;

    RandomGeneratorWorldBuilder& setPath(const std::string& path)
    {
        m_path = path;
        return *this;
    }

    RandomGeneratorWorldBuilder& setHeight(int64_t height)
    {
        m_height = height;
        return *this;
    }

    RandomGeneratorWorldBuilder& setWidth(int64_t width)
    {
        m_width = width;
        return *this;
    }

    RandomGeneratorWorldBuilder& setIterCount(int64_t iterCount)
    {
        m_iterCount = iterCount;
        return *this;
    }

    RandomGeneratorWorldBuilder& setDiscardByRatio(bool discardByRatio)
    {
        m_discardByRatio = discardByRatio;
        return *this;
    }

    RandomGeneratorWorldBuilder& setRoomVerticalRatio(double widthRatio)
    {
        m_widthRatio = widthRatio;
        return *this;
    }

    RandomGeneratorWorldBuilder& setRoomHorizontalRatio(float heightRatio)
    {
        m_heightRatio = heightRatio;
        return *this;
    }

private:

    struct Container
    {
        Container() = default;

        Container(const Container&) = default;
        Container(Container&&) = default;

        Container& operator=(const Container&) = default;
        Container& operator=(Container&&) = default;

        Container(Vec2Int _point, int64_t _w, int64_t _h)
            : point(_point)
            , center(point.x + _w/2, point.y + _h/2)
            , w(_w)
            , h(_h)
        {}

        Container(int64_t _x, int64_t _y, int64_t _w, int64_t _h)
                : point(_x, _y)
                , center(point.x + _w/2, point.y + _h/2)
                , w(_w)
                , h(_h)
        {}

        Vec2Int point;
        Vec2Int center;
        int64_t w = 0;
        int64_t h = 0;
    };

    struct Room
    {
        explicit Room(const Container& _cont) : cont(_cont)
        {
            point.x = cont.point.x + cocos2d::random(1l, cont.w/3);
            point.y = cont.point.y + cocos2d::random(1l, cont.h/3);
            w = cont.w - (point.x - cont.point.x);
            h = cont.h - (point.y - cont.point.y);

            w -= cocos2d::random(1l, w/3);
            h -= cocos2d::random(1l, h/3);
        }

        Container cont;
        Vec2Int point;
        int64_t h = 0;
        int64_t w = 0;
    };

    struct Tree
    {
        explicit Tree(const Container& _leaf) : leaf(_leaf) {}

        std::list<Container> getLeafs() const;
        std::list<std::pair<Vec2Int, Vec2Int>> getPaths() const;

        Container leaf;
        std::shared_ptr<Tree> left;
        std::shared_ptr<Tree> right;
    };

    std::pair<Container, Container> randomSplit(const Container& cont) const;
    std::shared_ptr<Tree> splitContainer(const Container& cont, size_t iterCount) const;

    void drawBackground(TilemapLayer* layer) const;
    void drawPaths(TilemapLayer* layer, const std::shared_ptr<Tree>& tree) const;
    void drawRooms(TilemapLayer* layer, const std::shared_ptr<Tree>& tree) const;

    Tilemap* generateWorld() const;

    std::string m_path;

    int64_t m_height = 0;
    int64_t m_width = 0;

    int64_t m_iterCount = 0;

    bool m_discardByRatio = true;

    double m_widthRatio = 0.45;
    double m_heightRatio = 0.45;
};
