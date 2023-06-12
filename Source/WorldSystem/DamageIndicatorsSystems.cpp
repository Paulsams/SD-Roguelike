#include "WorldSystem/DamageIndicatorsSystems.h"

#include <Utils/FontsTTF.h>
#include "WorldSystem/World.h"

DamageIndicator* DamageIndicator::create(World* world)
{
    auto* damageIndicator = new (std::nothrow) DamageIndicator(world);
    if (damageIndicator && damageIndicator->init())
    {
        damageIndicator->autorelease();
        return damageIndicator;
    }
    CC_SAFE_DELETE(damageIndicator);
    return nullptr;
}

bool DamageIndicator::init()
{
    m_sprite = cocos2d::Sprite::create(Paths::whiteCell.first);
    m_sprite->setTextureRect(Paths::whiteCell.second);
    m_sprite->setAnchorPoint(cocos2d::Vec2::ZERO);
    this->addChild(m_sprite);

    m_label = cocos2d::Label::createWithTTF("", FontsTTF::onDamageIndicator, 140);
    m_label->enableBold();
    m_label->enableShadow();
    m_label->setTextColor(cocos2d::Color4B::YELLOW);
    m_label->setScale(0.2f);
    m_world->addChild(m_label, 100);
    
    return true;
}

void DamageIndicator::setColorAndDamage(cocos2d::Color3B color, std::optional<float> damage) const
{
    m_sprite->setColor(color);
    m_label->setString(damage ? std::to_string(static_cast<int>(damage.value())) : "");
}

void DamageIndicator::setVisible(bool visible)
{
    Node::setVisible(visible);
    m_label->setVisible(visible);
}

void DamageIndicator::setPosition(const cocos2d::Vec2& position)
{
    setPosition(position.x, position.y);
}

void DamageIndicator::setPosition(float x, float y)
{
    Node::setPosition(x, y);
    m_label->setPosition(getPosition() + m_sprite->getContentSize() / 2);
}

DamageIndicator::DamageIndicator(World* world)
    : m_sprite(nullptr)
    , m_label(nullptr)
    , m_world(world) { }

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

void DamageIndicatorsSystems::draw(Vec2Int position, cocos2d::Color3B color, std::optional<float> damage)
{
    DamageIndicator* indicator = m_indicators.get();
    indicator->setVisible(true);
    indicator->setPosition(m_world->convertToMapSpace(position));
    indicator->setColorAndDamage(color, damage);
    m_showedObjects.push_back(indicator);
}

void DamageIndicatorsSystems::reset()
{
    for (DamageIndicator* indicator : m_showedObjects)
    {
        m_indicators.release(indicator);
        indicator->setVisible(false);
    }
    m_showedObjects.clear();
}

void DamageIndicatorsSystems::update()
{
    reset();
}

DamageIndicatorsSystems::DamageIndicatorsSystems(World* world)
    : m_indicators([this, world]()
        {
            DamageIndicator* damageIndicator = DamageIndicator::create(world);
            this->addChild(damageIndicator);
            return damageIndicator;
        })
    , m_world(world) { }
