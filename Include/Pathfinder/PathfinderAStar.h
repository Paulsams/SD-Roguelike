#pragma once
#include "Graph.h"
#include "IPathfindingAlgorithm.h"

namespace pathfinder {
    class PathfinderAStar : public IPathfindingAlgorithm {

        inline static float heuristic(Node* lhs, Node* rhs) {
            return lhs->distanceSquared(rhs);
        }
        
    public:
        std::vector<Vec2Int> findPath(Graph& graph, Node* start, Node* end) override;
    };

}
