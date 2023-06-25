#pragma once

#include "Node.h"
#include <functional>

namespace pathfinder {
    /**
     * Implements a graph that represents the terrain of the world
     */
    class Graph {
        // {y, x}
        inline static std::vector<std::pair<int, int>> direction {
                {-1, 0},
                {1, 0},
                {0, -1},
                {0, 1},
    //            {-1, -1},
    //            {1, -1},
    //            {-1, 1},
    //            {1, 1},
        };

        
        std::vector<Node> nodes;
        std::vector<bool> visited;
        std::vector<Node*> parents;

        cocos2d::Size worldSize;

    public:

        Graph(cocos2d::Size worldSize, uint32_t* tiles, std::function<TileType (int)> fToTileType) : worldSize(worldSize) {
            int totalSize = worldSize.width * worldSize.height;
            nodes.resize(totalSize);
            visited.resize(totalSize, false);
            parents.resize(totalSize, nullptr);
            initNodes(tiles, fToTileType);

        }

        /**
         * Create a vector of nodes from an array of tiles
         * @param tiles - a array of tiles
         * @param fToTileType - function returning tile type by tile id
         */
        void initNodes(uint32_t* tiles, const std::function<TileType (int)>& fToTileType) {
            int width = worldSize.width, height = worldSize.height;
            for(int x = 0; x < width; ++x) {
                for (int y = 0; y < height; ++y) {
                    Node curNode(Vec2Int(x, y), fToTileType(tiles[(height - 1 - y) * width + x/*y * width + x*/]));
                    for (auto[yy, xx]: direction) {
                        if (y - yy >= 0 && y - yy <= height - 1 && x + xx >= 0 && x + xx <= width - 1)
                            curNode.neighbors.push_back(&nodes[(y - yy) * width + (x + xx)]);
                    }

                    auto node = getNodeByPos(x, y);
                    *node = std::move(curNode);
                }
            }
        }

        /**
         * Get node position
         * @param node - graph node
         * @return position in the vector of nodes
         */
        int getNodePos(Node* node) const{
            return node->pos.y * worldSize.width + node->pos.x;
        }

        /**
         * Get node by position in vector
         * @param pos - Vec2Int position
         * @return Node in position
         */
        Node* getNodeByPos(Vec2Int pos) {
            return &nodes[pos.y * worldSize.width + pos.x];
        }
        /**
         * Get node by position in vector
         * @param x - x coordinate
         * @param y - y coordinate
         * @return Node in position
         */
        Node* getNodeByPos(int x, int y) {
            return &nodes[y * worldSize.width + x];
        }

        /**
         * Mark node as visited
         * @param node
         */
        void setVisitedNode(Node* node) {
            visited[getNodePos(node)] = true;
        }

        /**
         * Set parent of the passed Node
         * @param node - the node for which the parent is specified
         * @param parent - parent node
         */
        void setParentNode(Node* node, Node* parent) {
            parents[getNodePos(node)] = parent;
        }

        /**
         * Get visited status for node
         * @param node - Node
         * @return true if visited, otherwise false
         */
        bool getVisitedNode(Node* node) const {
            return visited[getNodePos(node)];
        }
        /**
         * Get parent for node
         * @param node - Node
         * @return parent node
         */
        Node* getParentNode(Node* node) const {
            return parents[getNodePos(node)];
        }

        /**
         * Clear all graph
         */
        void clear() noexcept {
            std::fill(visited.begin(), visited.end(), false);
            std::fill(parents.begin(), parents.end(), nullptr);

            for(auto& node: nodes) {
                node.clear();
            }
        }
    };

}