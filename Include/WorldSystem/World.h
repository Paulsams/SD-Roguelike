#pragma once

#include "BaseEntity.h"
#include "DamageIndicatorsSystems.h"
#include "FunctionVisitorEntities.h"
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
        m_ground->setCullingRect(rect);
        m_walls->setCullingRect(rect);
    }

    cocos2d::Vec2 convertToMapSpace(Vec2Int cellPosition) const
        { return cocos2d::Vec2(cellPosition.x * m_tilemap->getTileSize().width,
                               cellPosition.y * m_tilemap->getTileSize().height) * m_tilemap->getScale(); }

    Vec2Int convertToCellSpace(cocos2d::Vec2 position) const
        { return Vec2Int(position.x / m_tilemap->getTileSize().width,
                         position.y / m_tilemap->getTileSize().height); }

    void scheduleMove(BaseEntity* entity, BaseEntity::oldPosition oldPosition, BaseEntity::newPosition newPosition);

    static cocos2d::Rect getRectFromGid(int gid) { return cocos2d::Rect(
        Vec2Int{gid % 64, gid / 64} * 32, {32, 32}); }

    void addPlayer(Player* player);
    void removePlayer(Player* player);
    
    const Player* getNearestPlayer(cocos2d::Vec2 position) const;

    TileType getTileType(Vec2Int position) const;

    std::vector<Vec2Int> findPath(Vec2Int start, Vec2Int finish) const;

    void update() override;

    DamageIndicatorsSystems* getDamageIndicatorsForMobs() const { return m_damageIndicatorsMobs; }
    DamageIndicatorsSystems* getDamageIndicatorsForPlayer(const Player* player) const { return m_playersDamageIndicators.at(player); }

    EventContainer<const std::vector<mob::Mob*>> bossCountChanged;

private:
    explicit World(Tilemap* tilemap, std::shared_ptr<mob::BaseMobAbstractFactory> mobFactory);

    void tryInitDecorations(const cocos2d::TMXObjectGroup* decorationsGroup, cocos2d::Size tileSize);
    void tryInitChests(const cocos2d::TMXObjectGroup* chestsGroup, cocos2d::Size tileSize);
    bool spawnItem(Vec2Int position, const std::function<BaseItem*()>& createFunc);

    void checkMoveEntities();
    void trySpawnMobs(const cocos2d::TMXObjectGroup* group, cocos2d::Size tileSize,
                      const std::function<mob::Mob*(mob::BaseMobAbstractFactory*, World*, int)>& createFunc);

    void updateTileType(Vec2Int position) const;
    size_t getIndexFromVec2(Vec2Int position) const { return position.x + position.y * getSize().width; }
    
    TilemapLayer* m_ground = nullptr;
    TilemapLayer* m_walls = nullptr;

    std::shared_ptr<mob::BaseMobAbstractFactory> m_mobFactory;
    std::vector<mob::Mob*> m_mobs;
    std::vector<mob::Mob*> m_bosses;
    std::vector<std::vector<BaseEntity*>> m_entities;

    Player* m_player = nullptr;
    Tilemap* m_tilemap;
    Vec2Int m_spawnPoint;
    std::shared_ptr<pathfinder::Graph> m_graph;
    DamageIndicatorsSystems* m_damageIndicatorsMobs;
    std::map<const Player*, DamageIndicatorsSystems*> m_playersDamageIndicators;

    std::shared_ptr<IPathfindingAlgorithm> m_pathfinding;
    
    std::set<Vec2Int> m_currentMovedEntities;
    std::map<BaseEntity*, std::pair<Vec2Int, Vec2Int>> m_movedEntities;

    std::shared_ptr<FunctionVisitorEntitiesReturnVoid> m_visitorFromAddEntity;
    std::shared_ptr<FunctionVisitorEntitiesReturnVoid> m_visitorFromRemoveEntity;
    std::shared_ptr<FunctionVisitorEntities<bool>> m_visitorFindPath;
};
