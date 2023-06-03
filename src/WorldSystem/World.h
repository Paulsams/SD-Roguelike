#pragma once

#include <vector>
#include "BaseEntity.h"
#include "IWorldBuilder.h"
#include "2d/CCTMXTiledMap.h"

class World : public cocos2d::Node
{
public:
    static World* create(std::shared_ptr<IWorldBuilder> builder);
    
    void setBuilder(std::shared_ptr<IWorldBuilder> builder);
    bool generate();

    cocos2d::Point getSpawnPoint() const;
    
    bool tryGetEntity(cocos2d::Vec2 position, std::shared_ptr<const BaseEntity>& entity);
    cocos2d::Size getSize() const;
    cocos2d::Size getTileSize() const;

private:
    explicit World(std::shared_ptr<IWorldBuilder> builder);

    cocos2d::TMXTiledMap* m_tilemap = nullptr;
    
    std::vector<std::vector<std::shared_ptr<BaseEntity>>> m_grid;
    std::shared_ptr<IWorldBuilder> m_builder;
    cocos2d::Point m_spawnPoint;
};
