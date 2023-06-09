#include "WorldSystem/DamageIndicatorsSystems.h"

#include "WorldSystem/World.h"

DamageIndicatorsSystems* DamageIndicatorsSystems::create(World* world)
{
    DamageIndicatorsSystems* damageIndicators = new (std::nothrow) DamageIndicatorsSystems(world);
    if (damageIndicators && damageIndicators->init())
    {
        damageIndicators->autorelease();
        return damageIndicators;
    }
    CC_SAFE_DELETE(damageIndicators);
    return nullptr;
}

void DamageIndicatorsSystems::draw(Vec2Int position)
{
    cocos2d::Sprite* sprite = m_indicators.get();
    sprite->setVisible(true);
    sprite->setPosition(m_world->convertToMapSpace(position));
    m_showedObjects.push_back(sprite);
}

void DamageIndicatorsSystems::reset()
{
    for (cocos2d::Sprite* obj : m_showedObjects)
    {
        m_indicators.release(obj);
        obj->setVisible(false);
    }
    m_showedObjects.clear();
}

void DamageIndicatorsSystems::update()
{
    reset();
}

DamageIndicatorsSystems::DamageIndicatorsSystems(World* world)
    : m_indicators([this]()
        {
            cocos2d::Sprite* sprite = cocos2d::Sprite::create(Paths::whiteCell.first);
            sprite->setTextureRect(Paths::whiteCell.second);
            sprite->setAnchorPoint(cocos2d::Vec2::ZERO);
            sprite->setColor(cocos2d::Color3B::RED);
            this->addChild(sprite);
            
            return sprite;
        })
    , m_world(world) { }
