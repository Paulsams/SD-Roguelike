#pragma once

#include "BaseEntity.h"
#include "DamageIndicatorsSystems.h"
#include "GameLoop/IUpdatable.h"
#include "Mobs/Factory/BaseMobAbstractFactory.h"
#include "Pathfinder/Graph.h"
#include "Pathfinder/IPathfindingAlgorithm.h"
#include "Utils/Common.h"

class Player;
class BaseEntity;
class BaseItem;

class World : public cocos2d::Node, public IUpdatable
{
public:
    static World* create(Tilemap* tilemap, std::shared_ptr<mob::BaseMobAbstractFactory> mobFactory);

    bool initWithTilemap();

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

    cocos2d::Vec2 convertToMapSpace(Vec2Int cellPosition) const
        { return cocos2d::Vec2(cellPosition.x * m_tilemap->getTileSize().width,
                               cellPosition.y * m_tilemap->getTileSize().height) * m_tilemap->getScale(); }

    Vec2Int convertToCellSpace(cocos2d::Vec2 position) const
        { return Vec2Int(position.x / m_tilemap->getTileSize().width,
                         position.y / m_tilemap->getTileSize().height); }

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

    DamageIndicatorsSystems* getDamageIndicatorsForMobs() const { return m_damageIndicatorsMobs; }
    DamageIndicatorsSystems* getDamageIndicatorsForPlayer(const Player* player) const { return m_playersDamageIndicators.at(player); }
    
private:
    explicit World(Tilemap* tilemap, std::shared_ptr<mob::BaseMobAbstractFactory> mobFactory);

    void tryInitDecorations(const cocos2d::TMXObjectGroup* decorationsGroup, cocos2d::Size tileSize);
    void tryInitChests(const cocos2d::TMXObjectGroup* chestsGroup, cocos2d::Size tileSize);
    bool spawnItem(BaseEntity* entity, Vec2Int direction, const std::function<BaseItem*()>& createFunc);
    
    void trySpawnMobs(const cocos2d::TMXObjectGroup* group, cocos2d::Size tileSize,
        std::function<mob::Mob*(mob::BaseMobAbstractFactory*, World*, int)> createFunc);

    void updateTileType(Vec2Int position) const;
    size_t getIndexFromVec2(Vec2Int position) const { return position.x + position.y * getSize().width; }

    void onEntityMoved(BaseEntity* entity, BaseEntity::oldPosition oldPosition, BaseEntity::newPosition newPosition);
    FunctionHandler<BaseEntity*, BaseEntity::oldPosition, BaseEntity::newPosition> m_movedEntityDelegate;

    void internalRemoveEntity(BaseEntity* entity);
    void onDeletedEntity(BaseEntity* entity);

    FunctionHandler<BaseEntity*> m_deletedEntityDelegate;
    
    TilemapLayer* m_ground = nullptr;
    TilemapLayer* m_walls = nullptr;

    std::shared_ptr<mob::BaseMobAbstractFactory> m_mobFactory;
    std::vector<mob::Mob*> m_mobs;
    std::vector<std::vector<BaseEntity*>> m_entities;

    Player* m_player = nullptr;
    Tilemap* m_tilemap;
    Vec2Int m_spawnPoint;
    std::shared_ptr<pathfinder::Graph> m_graph;
    DamageIndicatorsSystems* m_damageIndicatorsMobs;
    std::map<const Player*, DamageIndicatorsSystems*> m_playersDamageIndicators;

    std::shared_ptr<IPathfindingAlgorithm> m_pathfinding;
    
    std::map<Vec2Int, BaseEntity*> m_movedEntities;
};
