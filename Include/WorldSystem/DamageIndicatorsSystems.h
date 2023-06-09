#pragma once

#include "2d/CCSprite.h"
#include "GameLoop/IUpdatable.h"
#include "Utils/Common.h"
#include "Utils/PoolObjects.h"

class World;

class DamageIndicatorsSystems : public cocos2d::Node, public IUpdatable
{
public:
    static DamageIndicatorsSystems* create(World* world);

    void draw(Vec2Int position);
    void reset();
    void update() override;

private:
    DamageIndicatorsSystems(World* world);
    
    std::vector<cocos2d::Sprite*> m_showedObjects;
    
    PoolObjects<cocos2d::Sprite*> m_indicators;
    World* m_world;
};
