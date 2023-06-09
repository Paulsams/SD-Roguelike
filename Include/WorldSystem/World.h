#pragma once

#include "BaseEntity.h"
#include "DamageIndicatorsSystems.h"
#include "GameLoop/IUpdatable.h"
#include "Pathfinder/Graph.h"
#include "Pathfinder/IPathfindingAlgorithm.h"
#include "Utils/Common.h"

class Player;
class BaseEntity;

enum EnemyType
{
    PASSIVE,
    NORMAL,
    ELITE,
    BOSS,
};

class World : public cocos2d::Node, public IUpdatable
{
public:
    static World* create(Tilemap* tilemap);

    bool initWithConfig();

    Vec2Int getSpawnPoint() const { return m_spawnPoint; }

    void addEntity(BaseEntity* entity);
    void removeEntity(BaseEntity* entity);
    
    const std::vector<BaseEntity*>& getEntitiesFromCell(Vec2Int position) const
        { return m_entities[getIndexFromVec2(position)]; }
    
    cocos2d::Size getSize() const { return m_tilemap->getMapSize(); }
    cocos2d::Size getTileSize() const { return m_tilemap->getTileSize(); }

    void updateCullingRect(cocos2d::Rect rect)
    {
//        m_background->setCullingRect(rect);
    }

    cocos2d::Vec2 convertToMapSpace(Vec2Int nodePosition)
        { return cocos2d::Vec2(nodePosition.x * m_tilemap->getTileSize().width,
            nodePosition.y * m_tilemap->getTileSize().height) * m_tilemap->getScale(); }

    static cocos2d::Rect getRectFromGid(int gid) { return cocos2d::Rect(
        Vec2Int{gid % 64, gid / 64} * 32, {32, 32}); }

    void addPlayer(Player* player);
    const Player* getNearestPlayer(cocos2d::Vec2 position) const;

    TileType getTileType(Vec2Int position) const;

    std::vector<Vec2Int> findPath(Vec2Int start, Vec2Int finish) const
    {
        return m_pathfinding->findPath(*m_graph, m_graph->getNodeByPos(start),
            m_graph->getNodeByPos(finish));
    }
    
    void update() override;

    DamageIndicatorsSystems* getDamageIndicatorsForPlayer() const { return m_damageIndicatorsPlayer; }
    DamageIndicatorsSystems* getDamageIndicatorsForEnemies() const { return m_damageIndicatorsEnemies; }
    
private:
    explicit World(Tilemap* tilemap);

    void updateTileType(Vec2Int position) const;

    size_t getIndexFromVec2(Vec2Int position) const { return position.x + position.y * getSize().width; }

    void onEntityMoved(BaseEntity::oldPosition oldPosition, BaseEntity::newPosition newPosition);
    FunctionHandler<BaseEntity::oldPosition, BaseEntity::newPosition> m_movedEntityDelegate;

    void onDeletedEntity(BaseEntity* entity);

    FunctionHandler<BaseEntity*> m_deletedEntityDelegate;
    
    TilemapLayer* m_ground;
    TilemapLayer* m_walls;

    std::vector<std::vector<BaseEntity*>> m_entities;

    Player* m_player = nullptr;
    Tilemap* m_tilemap;
    Vec2Int m_spawnPoint;
    std::shared_ptr<pathfinder::Graph> m_graph;
    DamageIndicatorsSystems* m_damageIndicatorsPlayer;
    DamageIndicatorsSystems* m_damageIndicatorsEnemies;

    std::shared_ptr<IPathfindingAlgorithm> m_pathfinding;
};
