#pragma once

#include <optional>

#include "2d/CCLabel.h"
#include "2d/CCSprite.h"
#include "GameLoop/IUpdatable.h"
#include "Utils/Common.h"
#include "Utils/PoolObjects.h"

class World;

class DamageIndicator : public cocos2d::Node
{
public:
    static DamageIndicator* create(World* world);
    
    bool init() override;

    void setColorAndDamage(cocos2d::Color3B color, std::optional<float> damage) const;
    void setVisible(bool visible) override;
    void setPosition(const cocos2d::Vec2& position) override;
    void setPosition(float x, float y) override;

private:
    DamageIndicator(World* world);

    cocos2d::Sprite* m_sprite;
    cocos2d::Label* m_label;
    World* m_world;
};

class DamageIndicatorsSystems : public cocos2d::Node, public IUpdatable
{
public:
    static DamageIndicatorsSystems* create(World* world);

    void draw(Vec2Int position, cocos2d::Color3B color, std::optional<float> damage);
    void reset();
    void update() override;

private:
    DamageIndicatorsSystems(World* world);
    
    std::vector<DamageIndicator*> m_showedObjects;
    
    PoolObjects<DamageIndicator*> m_indicators;
    World* m_world;
};
