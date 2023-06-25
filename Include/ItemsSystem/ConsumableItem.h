#pragma once

#include "ItemsSystem/BaseItem.h"


/**
 * Consumable item class
 */
class ConsumableItem final : public BaseItem
{
public:
    /**
     * Create consumable item with given image position in the tileset and interaction function
     * @param world world
     * @param rect image pixel position in the tileset
     * @param isInteractedFunc interaction function
     * @return ConsumableItem
     */
    static ConsumableItem* create(World* world, const cocos2d::Rect& rect, const std::function<bool(const Player*)>& isInteractedFunc)
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

    /**
     * @return item type of this item
     */
    ItemTypeSlot getItemTypeFromSlot() const override { return CONSUMABLE; }

    /**
     * @return stats of this item
     */
    const std::shared_ptr<IStatsContainer> getStats() const override { return m_stats; }

    /**
     * Call interaction function
     * @return true if successful, false otherwise
     */
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
        , m_isInteracted(isInteractedFunc)
    {}
    
    std::shared_ptr<StatsContainer> m_stats;
    std::function<bool (const Player*)> m_isInteracted;
};
