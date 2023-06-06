#pragma once
#include <vector>

#include "Pathfinder/Graph.h"
#include "Utils/Common.h"

struct IPathfindingAlgorithm
{
    virtual ~IPathfindingAlgorithm() = default;
    virtual std::vector<Vec2Int> findPath(pathfinder::Graph& graph, pathfinder::Node* start, pathfinder::Node* end) = 0;
};
