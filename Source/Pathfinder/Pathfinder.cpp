#include "Pathfinder/Pathfinder.h"

namespace pathfinder {

       std::deque<cocos2d::Point> Pathfinder::findPath(Graph& graph, Node* start, Node* end) {
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
        while (!queue.empty() && curNode != end) {

            while(!queue.empty() && graph.getVisitedNode(queue.top()))
                queue.pop();

            curNode = queue.top();
            graph.setVisitedNode(curNode);

            for(auto neighbor: curNode->neighbors) {
                if (!graph.getVisitedNode(neighbor) && neighbor->tile != TILE_TYPE::LET) {
                    queue.push(neighbor);
                }

                float possibleLowerLocalGoal = curNode->fLocalGoal + curNode->distance(neighbor);
                if (possibleLowerLocalGoal < neighbor->fLocalGoal) {
                    graph.setParentNode(neighbor, curNode);
                    neighbor->fLocalGoal = possibleLowerLocalGoal;

                    neighbor->fGlobalGoal = neighbor->fLocalGoal + heuristic(neighbor, end);
                }
            }
        }

        std::deque<cocos2d::Point> path;
        path.push_front(end->pos);
        Node* parent = nullptr;
        while (!(parent = graph.getParentNode(end))) {
            path.push_front(parent->pos);
            end = parent;
        }

        return path;
    }

}