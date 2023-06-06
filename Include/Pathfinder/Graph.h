#pragma once
#include "Node.h"
#include <functional>

namespace pathfinder {
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

        Graph(cocos2d::Size worldSize, uint32_t* tiles, std::function<TILE_TYPE(uint32_t)> fToTileType) : worldSize(worldSize) {
            int totalSize = worldSize.width * worldSize.height;
            nodes.resize(totalSize);
            visited.resize(totalSize, false);
            parents.resize(totalSize, nullptr);
            initNodes(tiles, fToTileType);

        }

        void initNodes(uint32_t* tiles, const std::function<TILE_TYPE(uint32_t)>& fToTileType) {
            int width = worldSize.width, height = worldSize.height;
            for(int x = 0; x < width; ++x) {
                for (int y = 0; y < height; ++y) {
                    Node curNode(cocos2d::Point(x, y), fToTileType(tiles[y * width + x]));
                    for (auto[yy, xx]: direction) {
                        if (y > 0 && y < height - 1 && x > 0 && x < width - 1)
                            curNode.neighbors.push_back(&nodes[(y - yy) * width + (x + xx)]);
                    }

                    auto node = getNodeByPos(x, y);
                    *node = std::move(curNode);
                }
            }
        }

        int getNodePos(Node* node) const{
            return node->pos.y * worldSize.width + node->pos.x;
        }

        Node* getNodeByPos(Vec2Int pos) {
            return &nodes[pos.y * worldSize.width + pos.x];
        }

        Node* getNodeByPos(int x, int y) {
            return &nodes[y * worldSize.width + x];
        }

        void setVisitedNode(Node* node) {
            visited[getNodePos(node)] = true;
        }

        void setParentNode(Node* node, Node* parent) {
            parents[getNodePos(node)] = parent;
        }
        
        bool getVisitedNode(Node* node) const {
            return visited[getNodePos(node)];
        }

        Node* getParentNode(Node* node) const {
            return parents[getNodePos(node)];
        }


        void clear() noexcept {
            std::fill(visited.begin(), visited.end(), false);
            std::fill(parents.begin(), parents.end(), nullptr);

            for(auto& node: nodes) {
                node.clear();
            }
        }
    };

}