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
    /**
     * Static method for creating a world \n
     * Additionally puts a new object in autorealease queue
     * @param tilemap map on which the world is based
     * @param mobFactory factory for creating mobs
     * @return new world
     */
    static World* create(Tilemap* tilemap, std::shared_ptr<mob::BaseMobAbstractFactory> mobFactory);

    /**
     * @return starting point for the player to spawn
     */
    Vec2Int getSpawnPoint() const { return m_spawnPoint; }

    /**
     * Adds an entity to the world and is now watched by the given world
     * @param entity entity
     */
    void addEntity(BaseEntity* entity);
    /**
     * Remove an entity to the world
     * @param entity entity
     */
    void removeEntity(BaseEntity* entity);
    
    /**
     * @param position position relative to the map
     * @return vector of entities currently located in the given cell
     */
    const std::vector<BaseEntity*>& getEntitiesFromCell(Vec2Int position) const
        { return m_entities[getIndexFromVec2(position)]; }
    
    /**
     * @return map size
     */
    cocos2d::Size getSize() const { return m_tilemap->getMapSize(); }
    /**
     * @return tile size
     */
    cocos2d::Size getTileSize() const { return m_tilemap->getTileSize(); }

    /**
     * Updates the rectangle outside of which no tiles are drawn
     * @param rect describes the area in which tiles are drawn
     */
    void updateCullingRect(cocos2d::Rect rect)
    {
        m_ground->setCullingRect(rect);
        m_walls->setCullingRect(rect);
    }

    /**
     * Returns global position based on the given position on the map
     * @param cellPosition position relative to the map
     * @return global position
     */
    cocos2d::Vec2 convertToMapSpace(Vec2Int cellPosition) const
        { return cocos2d::Vec2(cellPosition.x * m_tilemap->getTileSize().width,
                               cellPosition.y * m_tilemap->getTileSize().height) * m_tilemap->getScale(); }

    /**
     * Returns position on the map based on the global position
     * @param position global position
     * @return position relative to the map
     */
    Vec2Int convertToCellSpace(cocos2d::Vec2 position) const
        { return Vec2Int(position.x / m_tilemap->getTileSize().width,
                         position.y / m_tilemap->getTileSize().height); }

    /**
     * Schedules movement to update them after all entities in the current iteration have updated
     * @param entity entity to be moved later
     * @param oldPosition start position
     * @param newPosition end position
     */
    void scheduleMove(BaseEntity* entity, BaseEntity::OldPosition oldPosition, BaseEntity::NewPosition newPosition);

    /**
     * Allows you to get a rectangle in which there will be a sprite for drawing by the number in the tileset
     * @param gid number in the tileset
     * @return a rectangle describing the sprite from the tileset saved to the world
     */
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
    bool initWithTilemap();
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
    std::list<mob::Mob*> m_mobs;
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
