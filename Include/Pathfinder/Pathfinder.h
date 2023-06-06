#pragma once
#include "Graph.h"
#include <queue>
#include <deque>

namespace pathfinder {


    class Pathfinder {

        inline static double heuristic(Node* lhs, Node* rhs) {
            return lhs->distance(rhs);
        }
    public:
        static std::deque<cocos2d::Point> findPath(Graph& graph, Node* start, Node* end);

    };

}