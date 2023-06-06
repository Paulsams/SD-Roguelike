#pragma once

#include "BaseEntity.h"
#include "Utils/Common.h"

class Player;
class BaseEntity;

enum class TileType : uint32_t {
    GROUND = 0,
    OBSTACLE = 1,
    MOB = 2,
    OUT_OF_BOUNDS = 3,
};

class World : public cocos2d::Node
{
public:
    static World* create(Tilemap* tilemap);

    bool init() override;

    Vec2Int getSpawnPoint() const { return m_spawnPoint; }

    void addEntity(BaseEntity* entity);
    void removeEntity(BaseEntity* entity);
    
    bool tryGetEntity(Vec2Int position, const BaseEntity*& entity) const;
    cocos2d::Size getSize() const;
    cocos2d::Size getTileSize() const;

    void updateCullingRect(cocos2d::Rect rect)
    {
//        m_background->setCullingRect(rect);
    }

    cocos2d::Vec2 convertToWorldSpace(Vec2Int nodePosition)
        { return cocos2d::Vec2(nodePosition.x * m_tilemap->getTileSize().width,
            nodePosition.y * m_tilemap->getTileSize().height) * m_tilemap->getScale(); }

    void addPlayer(Player* player);
    const Player* getNearestPlayer(cocos2d::Vec2 position) const;

    TileType getTileType(Vec2Int position) const;

    // std::vector<cocos2d::Point> searchPath(cocos2d::Vec2Int start, cocos2d::Vec2Int finish)
    // {
    //     m_pathfindingAlgorithm.search(graph, Node(), Node());
    // }

private:
    explicit World(Tilemap* tilemap);

    void onEntityMoved(BaseEntity::oldPosition oldPosition, BaseEntity::newPosition newPosition);
    FunctionHandler<BaseEntity::oldPosition, BaseEntity::newPosition> m_movedEntityDelegate;

    void onDeletedEntity(BaseEntity* entity);
    FunctionHandler<BaseEntity*> m_deletedEntityDelegate;
    
    TilemapLayer* m_background;

    std::shared_ptr<BaseEntity*[]> m_entities;

    Player* m_player = nullptr;
    Tilemap* m_tilemap;
    Vec2Int m_spawnPoint;
    
    // IPathfindingAlgorithm m_pathfinding;
};
