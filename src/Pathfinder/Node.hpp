#pragma once
#include <cocos2d.h>
#include <vector>
#include <numeric>

namespace pathfinder {

    enum class TILE_TYPE : uint32_t {
        GROUND = 0,
        LET = 1,
    };


    struct Node {
        cocos2d::Point pos;
        TILE_TYPE tile = TILE_TYPE::GROUND;
        std::vector<Node *> neighbors;

        float fGlobalGoal = std::numeric_limits<float>::infinity();
        float fLocalGoal = std::numeric_limits<float>::infinity();

        Node() : pos(-1, -1) {}
        Node(const cocos2d::Point& pos,  TILE_TYPE tile) : pos(pos), tile(tile) {}
        Node(const cocos2d::Point& pos) : pos(pos) {}

        void clear() noexcept {
            fGlobalGoal = std::numeric_limits<float>::infinity();
            fLocalGoal = std::numeric_limits<float>::infinity();
        }

        float distance(const Node* other) {
            return pos.distance(other->pos);
        }
    };
}