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
            , w(_w)
            , h(_h)
            , center(point.x + _w/2, point.y + _h/2)
        {}

        Container(int64_t _x, int64_t _y, int64_t _w, int64_t _h)
                : point(_x, _y)
                , w(_w)
                , h(_h)
                , center(point.x + _w/2, point.y + _h/2)
        {}

        Vec2Int point;
        Vec2Int center;
        int64_t h = 0;
        int64_t w = 0;
    };

    struct Room
    {
        Room(const Container& _cont) : cont(_cont)
        {
            point.x = cont.point.x + cocos2d::random(0l, cont.w/3);
            point.y = cont.point.y + cocos2d::random(0l, cont.h/3);
            w = cont.w - (point.x - cont.point.x);
            h = cont.h - (point.y - cont.point.y);

            w -= cocos2d::random(0l, w/3);
            h -= cocos2d::random(0l, h/3);
        }

        Container cont;
        Vec2Int point;
        int64_t h = 0;
        int64_t w = 0;
    };

    struct Tree
    {
        Tree(const Container& _leaf) : leaf(_leaf) {}

        std::list<Container> getLeafs() const
        {
            if (!left && ! right)
                return {leaf};
            std::list<Container> res = left->getLeafs();
            res.splice(res.end(), right->getLeafs());
            return res;
        }
        Container leaf;
        std::shared_ptr<Tree> left;
        std::shared_ptr<Tree> right;
    };

    std::pair<Container, Container> randomSplit(const Container& cont) const
    {
        Container c1;
        Container c2;
        if (cocos2d::random() % 2 == 0)
        {
            // Vertical
            c1 = Container{cont.point, cocos2d::random(1l, cont.w), cont.h};
            c2 = Container{cont.point.x + c1.w, cont.point.y, cont.w - c1.w, cont.h};

            if (m_discardByRatio)
            {
                double c1WR = static_cast<double>(c1.w) / c1.h;
                double c2WR = static_cast<double>(c2.w) / c2.h;
                if (c1WR < m_widthRatio || c2WR < m_widthRatio)
                    return randomSplit(cont);
            }
        }
        else
        {
            // Horizontal
            c1 = Container{cont.point, cont.w, cocos2d::random(1l, cont.h)};
            c2 = Container{cont.point.x, cont.point.y + c1.h, cont.w, cont.h - c1.h};

            if (m_discardByRatio)
            {
                double c1HR = static_cast<double>(c1.h) / c1.w;
                double c2HR = static_cast<double>(c2.h) / c2.w;
                if (c1HR < m_heightRatio || c2HR < m_heightRatio)
                    return randomSplit(cont);
            }
        }
        return std::make_pair(c1, c2);
    }

    std::shared_ptr<Tree> splitContainer(const Container& cont, size_t iterCount) const
    {
        auto root = std::make_shared<Tree>(cont);
        if (iterCount)
        {
            auto [c1, c2] = randomSplit(cont);
            root->left = std::move(splitContainer(c1, iterCount - 1));
            root->right = std::move(splitContainer(c2, iterCount - 1));
        }
        return root;
    }

    cocos2d::TMXTiledMap* generateWorld() const
    {
        cocos2d::TMXTiledMap* tiledMap = cocos2d::TMXTiledMap::create(m_path);
        Container mainContainer(0, 0, m_width, m_height);
        std::shared_ptr<Tree> tree = splitContainer(mainContainer, m_iterCount);


        return nullptr;
    }


    std::string m_path;

    int64_t m_height = 0;
    int64_t m_width = 0;
    int64_t m_iterCount = 0;
    bool m_discardByRatio = true;

    double m_widthRatio = 0.45;
    double m_heightRatio = 0.45;
};
