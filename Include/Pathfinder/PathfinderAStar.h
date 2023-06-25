#pragma once
#include "Graph.h"
#include "IPathfindingAlgorithm.h"

namespace pathfinder {
    /**
     * Implements the pathfinding algorithm A*
     */
    class PathfinderAStar : public IPathfindingAlgorithm {

        inline static float heuristic(Node* lhs, Node* rhs) {
            return lhs->distanceSquared(rhs);
        }
        
    public:
        /**
         * Finding the shortest path between two nodes
         */
        std::vector<Vec2Int> findPath(Graph& graph, Node* start, Node* end,
            const std::function<bool(const Node*)>& isValidMove) override;
    };

}
