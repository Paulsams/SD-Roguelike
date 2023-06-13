#include "Pathfinder/PathfinderAStar.h"

#include "Utils/Common.h"

namespace pathfinder {

    std::vector<Vec2Int> PathfinderAStar::findPath(Graph& graph, Node* start, Node* end)
    {
        if (!start || !end)
            throw std::invalid_argument("start or end is null");

        if (start == end)
            return {start->pos};

        graph.clear();
        Node* curNode = start;
        start->fLocalGoal = 0;
        start->fGlobalGoal = heuristic(start, end);

        auto comporator = [](const Node* lhs, const Node* rhs) { return lhs->fGlobalGoal > rhs->fGlobalGoal;};
        std::priority_queue<Node*, std::vector<Node*>, decltype(comporator)> queue(comporator);
        queue.push(start);
        while (!queue.empty() && curNode != end)
        {

            while(!queue.empty() && graph.getVisitedNode(queue.top()))
                queue.pop();

            if (queue.empty())
                break;

            curNode = queue.top();
            graph.setVisitedNode(curNode);

            for(auto neighbor: curNode->neighbors)
            {
                if (!graph.getVisitedNode(neighbor) && neighbor->tile != TileType::OBSTACLE &&
                    neighbor->tile != TileType::DECORATION)
                {
                    queue.push(neighbor);
                }

                float possibleLowerLocalGoal = curNode->fLocalGoal + curNode->distanceSquared(neighbor);
                if (possibleLowerLocalGoal < neighbor->fLocalGoal)
                    {
                    graph.setParentNode(neighbor, curNode);
                    neighbor->fLocalGoal = possibleLowerLocalGoal;

                    neighbor->fGlobalGoal = neighbor->fLocalGoal + heuristic(neighbor, end);
                }
            }
        }

        std::vector<Vec2Int> path;
        Node* parent;
        while ((parent = graph.getParentNode(end)))
        {
            path.push_back(end->pos);
            end = parent;
        }

        std::reverse(path.begin(), path.end());
        return path;
    }

}
