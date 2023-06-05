#pragma once

#include "BaseEntity.h"
#include "2d/CCFastTMXLayer.h"
#include "Utils/Common.h"

class Player;

enum class TileType : uint32_t {
    Ground = 0,
    Obstacle = 1,
};

class World : public cocos2d::Node
{
public:
    static World* create(Tilemap* tilemap);

    bool init() override;

    cocos2d::Point getSpawnPoint() const;

    //void addEntity(BaseEntity entity);
    //void removeEntity(BaseEntity entity);
    
    bool tryGetEntity(Vec2Int position, std::shared_ptr<const BaseEntity>& entity);
    cocos2d::Size getSize() const;
    cocos2d::Size getTileSize() const;

    void updateCullingRect(cocos2d::Rect rect)
    {
//        m_background->setCullingRect(rect);
    }

    void addPlayer(Player* player);
    const Player* getNearestPlayer(cocos2d::Vec2 position) const;

    TileType getTileType(Vec2Int position);

    // std::vector<cocos2d::Point> searchPath(cocos2d::Vec2Int start, cocos2d::Vec2Int finish)
    // {
    //     m_pathfindingAlgorithm.search(graph, Node(), Node());
    // }

private:
    explicit World(Tilemap* tilemap);
    TilemapLayer* m_background;

    const std::vector<std::shared_ptr<BaseEntity>> m_entities;

    Player* m_player = nullptr;
    Tilemap* m_tilemap;
    cocos2d::Point m_spawnPoint;
    
    // IPathfindingAlgorithm m_pathfinding;
};
