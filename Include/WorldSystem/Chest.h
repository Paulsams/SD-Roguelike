#pragma once
#include "BaseEntity.h"
#include "IVisitorEntities.h"
#include "2d/CCSprite.h"

class Chest final : public BaseEntity
{
public:
    static Chest* create(World* world, const SpriteWithRect& spriteInfo,
        const std::function<void(Chest*)>& onInteractedFunc)
    {
        auto chest = new (std::nothrow) Chest(world, onInteractedFunc);
        if (chest && chest->initWithSpriteInfo(spriteInfo))
        {
            chest->autorelease();
            return chest;
        }
        CC_SAFE_DELETE(chest);
        return nullptr;
    }

    bool initWithSpriteInfo(const SpriteWithRect& spriteInfo)
    {
        m_sprite = cocos2d::Sprite::create(spriteInfo.first, spriteInfo.second);
        m_sprite->setAnchorPoint(cocos2d::Vec2::ZERO);
        this->addChild(m_sprite);

        return true;
    }
    
    const std::shared_ptr<IStatsContainer> getStats() const override { return m_stats; }
    void acceptVisit(std::shared_ptr<IVisitorEntities> visitor) override { visitor->visitChest(this); }

    void interact()
    {
        if (m_isOpen)
            return;

        m_isOpen = true;
        m_onInteractedFunc(this);
        destroyEntity(); 
    }

private:
    explicit Chest(World* world, const std::function<void(Chest*)>& onInteractedFunc)
        : BaseEntity(world)
        , m_onInteractedFunc(onInteractedFunc)
        , m_stats(std::make_shared<StatsContainer>()) { }

    std::function<void(Chest*)> m_onInteractedFunc;
    std::shared_ptr<StatsContainer> m_stats;

    cocos2d::Sprite* m_sprite = nullptr;
    bool m_isOpen = false;
};
