#pragma once
#include "BaseEntity.h"
#include "IVisitorEntities.h"
#include "2d/CCSprite.h"

/**
 * An entity that allows the player to interact with it and do something with this interaction
 */
class Chest final : public BaseEntity
{
public:
    /**
     * Static method for creating a chest \n
     * Additionally puts a new object in autorealease queue
     * @param world world in which this entity was created
     * @param spriteInfo pair from the path to tileset and rect in it
     * @param onInteractedFunc functor called when interacting with the chest
     * @return new chest
     */
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
    
    /**
     * @return Stats Container
     */
    const std::shared_ptr<IStatsContainer> getStats() const override { return m_stats; }

    /**
     * Accept visitor with given function
     * @param visitor visitor
     */
    void acceptVisit(std::shared_ptr<IVisitorEntities> visitor) override { visitor->visitChest(this); }

    /**
     * Interaction with chest
     */
    void interact()
    {
        if (m_isOpen)
            return;

        m_isOpen = true;
        m_onInteractedFunc(this);
    }

private:
    bool initWithSpriteInfo(const SpriteWithRect& spriteInfo)
    {
        m_sprite = cocos2d::Sprite::create(spriteInfo.first, spriteInfo.second);
        m_sprite->setAnchorPoint(cocos2d::Vec2::ZERO);
        this->addChild(m_sprite);

        return true;
    }
    
    explicit Chest(World* world, const std::function<void(Chest*)>& onInteractedFunc)
        : BaseEntity(world)
        , m_onInteractedFunc(onInteractedFunc)
        , m_stats(std::make_shared<StatsContainer>()) { }

    std::function<void(Chest*)> m_onInteractedFunc;
    std::shared_ptr<StatsContainer> m_stats;

    cocos2d::Sprite* m_sprite = nullptr;
    bool m_isOpen = false;
};
