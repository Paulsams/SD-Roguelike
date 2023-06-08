#pragma once

#include "Stats/Modificators/StatWithModificators.h"
#include "WorldSystem/BaseEntity.h"
#include "WorldSystem/IVisitorEntities.h"

class Decoration : public BaseEntity
{
public:
    static Decoration* create(World* world, cocos2d::Rect rect)
    {
        Decoration* decoration = new (std::nothrow) Decoration(world);
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
        cocos2d::Sprite* sprite = cocos2d::Sprite::create(Paths::toGameTileset, rect);
        sprite->setAnchorPoint(cocos2d::Vec2::ZERO);
        this->addChild(sprite);

        return true;
    }

    const std::shared_ptr<IStatsContainer> getStats() const override { return m_stats; }
    
    void acceptVisit(std::shared_ptr<IVisitorEntities> visitor) override { visitor->visitDecoration(this); }

private:
    explicit Decoration(World* world)
        : BaseEntity(world)
        , m_stats(std::make_shared<StatsContainer>())
    {
        m_stats->add(Health, std::make_shared<StatWithModificators>(1.0f));
    }

    std::shared_ptr<StatsContainer> m_stats;
};
