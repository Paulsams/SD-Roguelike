#pragma once

#include "Pathfinder/Graph.h"
#include "Pathfinder/IPathfindingAlgorithm.h"

namespace pathfinder
{

/**
 * Implementation of the pathfinding algorithm A*
 */
class PathfinderAStar : public IPathfindingAlgorithm
{

    inline static float heuristic(Node* lhs, Node* rhs)
    {
        return lhs->distanceSquared(rhs);
    }

public:
    /**
     * Find the shortest path between two nodes
     */
    std::vector<Vec2Int> findPath(Graph& graph,
                                  Node* start,
                                  Node* end,
                                  const std::function<bool(const Node*)>& isValidMove) override;
};

}
