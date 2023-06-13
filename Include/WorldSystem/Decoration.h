#pragma once

#include <Stats/Modificators/BoundsModificator.h>

#include "2d/CCSprite.h"
#include "Stats/Modificators/StatWithModificators.h"
#include "WorldSystem/BaseEntity.h"
#include "WorldSystem/IVisitorEntities.h"

class Decoration : public BaseEntity
{
public:
    static Decoration* create(World* world, cocos2d::Rect rect)
    {
        auto* decoration = new (std::nothrow) Decoration(world);
        if (decoration && decoration->initWithRect(rect))
        {
            decoration->autorelease();
            return decoration;
        }
        CC_SAFE_DELETE(decoration);
        return nullptr;
    }

    bool initWithRect(const cocos2d::Rect& rect)
    {
        m_sprite = cocos2d::Sprite::create(Paths::toGameTileset, rect);
        m_sprite->setAnchorPoint(cocos2d::Vec2::ZERO);
        this->addChild(m_sprite);

        return true;
    }

    const std::shared_ptr<IStatsContainer> getStats() const override { return m_stats; }
    
    void acceptVisit(std::shared_ptr<IVisitorEntities> visitor) override { visitor->visitDecoration(this); }

private:
    explicit Decoration(World* world)
        : BaseEntity(world)
        , m_stats(std::make_shared<StatsContainer>())
    {
        static constexpr float health = 1.0f;
        
        std::shared_ptr<StatWithModificators> healthStat = std::make_shared<StatWithModificators>(health);
        healthStat->addModificator(std::make_shared<BoundsModificator>(MinMax(0, health)));
        m_stats->add(HEALTH, healthStat);
        healthStat->changed += [this](IStat::oldValue, IStat::currentValue current, IStat::changedValue)
        {
            if (current <= 0.0f)
                destroyEntity();
        };
    }

    std::shared_ptr<StatsContainer> m_stats;
    cocos2d::Sprite* m_sprite = nullptr;
};
