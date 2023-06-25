#pragma once
#include <cocos2d.h>
#include <vector>
#include <numeric>

#include "Utils/Common.h"

enum class TileType {
    GROUND = 0,
    OBSTACLE = 1,
    DECORATION = 2,
    OUT_OF_BOUNDS = 3,
};

namespace pathfinder {
    /**
     * Graph node, contains graph position, tile type and list of neighbour nodes
     */
    struct Node {
        Vec2Int pos;
        TileType tile = TileType::GROUND;
        std::vector<Node *> neighbors;

        float fGlobalGoal = std::numeric_limits<float>::infinity();
        float fLocalGoal = std::numeric_limits<float>::infinity();

        Node() : pos(-1, -1) {}
        Node(const cocos2d::Point& pos, TileType tile) : pos(pos), tile(tile) {}
        Node(const cocos2d::Point& pos) : pos(pos) {}

        /**
         * Clear node
         */
        void clear() noexcept {
            fGlobalGoal = std::numeric_limits<float>::infinity();
            fLocalGoal = std::numeric_limits<float>::infinity();
        }

        /**
         * Calculate distance between nodes
         * @param other - other Node
         * @return float distance
         */
        float distanceSquared(const Node* other) {
            return pos.distanceSquared(other->pos);
        }
    };
}
