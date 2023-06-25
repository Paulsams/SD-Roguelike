#pragma once
#include <vector>

#include "Pathfinder/Graph.h"
#include "Utils/Common.h"

/**
 * Path finding algorithm interface
 */
struct IPathfindingAlgorithm
{
    virtual ~IPathfindingAlgorithm() = default;
    /**
     * Finding the shortest path between two nodes
     * @param graph - the graph in witch the path is searched
     * @param start - start Node
     * @param end  - end Node
     * @param isValidMove - a function that determines whether a transition is possible
     * @return shortest path between two nodes
     */
    virtual std::vector<Vec2Int> findPath(pathfinder::Graph& graph, pathfinder::Node* start, pathfinder::Node* end,
            const std::function<bool(const pathfinder::Node*)>& isValidMove) = 0;
};
