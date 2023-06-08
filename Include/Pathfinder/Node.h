#pragma once
#include <cocos2d.h>
#include <vector>
#include <numeric>

#include "Utils/Common.h"

enum class TileType {
    GROUND = 0,
    OBSTACLE = 1,
    OUT_OF_BOUNDS = 2,
};

namespace pathfinder {
    
    struct Node {
        Vec2Int pos;
        TileType tile = TileType::GROUND;
        std::vector<Node *> neighbors;

        float fGlobalGoal = std::numeric_limits<float>::infinity();
        float fLocalGoal = std::numeric_limits<float>::infinity();

        Node() : pos(-1, -1) {}
        Node(const cocos2d::Point& pos, TileType tile) : pos(pos), tile(tile) {}
        Node(const cocos2d::Point& pos) : pos(pos) {}

        void clear() noexcept {
            fGlobalGoal = std::numeric_limits<float>::infinity();
            fLocalGoal = std::numeric_limits<float>::infinity();
        }

        float distanceSquared(const Node* other) {
            return pos.distanceSquared(other->pos);
        }
    };
}
