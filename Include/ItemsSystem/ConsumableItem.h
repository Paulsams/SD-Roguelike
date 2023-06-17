#pragma once
#include "BaseItem.h"

class ConsumableItem final : public BaseItem
{
public:
    static ConsumableItem* create(World* world, const cocos2d::Rect& rect,
        const std::function<bool(const Player*)>& isInteractedFunc)
    {
        auto item = new (std::nothrow) ConsumableItem(world, rect, isInteractedFunc);
        if (item && item->init())
        {
            item->autorelease();
            return item;
        }
        CC_SAFE_DELETE(item);
        return nullptr;
    }
    
    ItemTypeSlot getItemTypeFromSlot() const override { return CONSUMABLE; }
    const std::shared_ptr<IStatsContainer> getStats() const override { return m_stats; }

    bool interact() override
    {
        const Player* player = getPlayer();
        if (!player)
            return false;

        return m_isInteracted(player);
    }

private:
    ConsumableItem(World* world, const cocos2d::Rect& rect, const std::function<bool(const Player*)>& isInteractedFunc)
        : BaseItem(world, rect)
        , m_isInteracted(isInteractedFunc) { }
    
    std::shared_ptr<StatsContainer> m_stats;
    std::function<bool (const Player*)> m_isInteracted;
};
