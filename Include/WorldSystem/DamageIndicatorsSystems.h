#pragma once

#include <optional>

#include "2d/CCLabel.h"
#include "2d/CCSprite.h"
#include "GameLoop/IUpdatable.h"
#include "Utils/Common.h"
#include "Utils/PoolObjects.h"

class World;

struct DrawDamageInfo
{
    const Vec2Int position;
    const cocos2d::Color3B color;
    const std::optional<float> damage;
};

class DamageIndicator : public cocos2d::Node
{
public:
    static DamageIndicator* create(World* world);
    
    bool init() override;

    void setColorAndDamage(cocos2d::Color3B color, std::optional<float> damage);
    void changeDamageValueBy(float changeValue);
    
    void setVisible(bool visible) override;
    void setPosition(const cocos2d::Vec2& position) override;
    void setPosition(float x, float y) override;

private:
    DamageIndicator(World* world);

    float m_currentDamage;
    cocos2d::Sprite* m_sprite;
    cocos2d::Label* m_label;
    World* m_world;
};

class DamageIndicatorsSystems : public cocos2d::Node, public IUpdatable
{
public:
    static DamageIndicatorsSystems* create(World* world);

    void scheduleDraw(const std::function<void(std::function<void(DrawDamageInfo)>)>& scheduleDrawFunc);
    void reset();
    void update() override;

private:
    DamageIndicatorsSystems(World* world);
    
    std::map<Vec2Int, DamageIndicator*> m_showedObjects;
    PoolObjects<DamageIndicator> m_indicators;
    World* m_world;

    std::vector<std::function<void(std::function<void(DrawDamageInfo)>)>> m_deferredDraws;
};
