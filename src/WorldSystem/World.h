#pragma once

#include "2d/CCTMXTiledMap.h"

class World : public cocos2d::Node
{
public:
    static World* create(cocos2d::TMXTiledMap* tilemap);

    bool init() override;

    cocos2d::Point getSpawnPoint() const;
    
    bool tryGetEntity(cocos2d::Vec2 position, std::shared_ptr<const Node>& entity);
    cocos2d::Size getSize() const;
    cocos2d::Size getTileSize() const;

    // std::vector<cocos2d::Point> searchPath(cocos2d::Vec2 start, cocos2d::Vec2 finish)
    // {
    //     m_pathfindingAlgorithm.search(graph, Node(), Node());
    // }

private:
    explicit World(cocos2d::TMXTiledMap* tilemap);

    cocos2d::TMXTiledMap* m_tilemap;
    cocos2d::Point m_spawnPoint;
    
    // IPathfindingAlgorithm m_pathfinding;
};
