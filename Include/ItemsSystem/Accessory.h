#pragma once

#include "BaseItem.h"


/**
 * Accessory item
 *
 * Gives passive stats for the player (owner) if worn in an active slot
 */
class Accessory final : public BaseItem
{
public:
    Accessory(World* world, const cocos2d::Rect& rect)
        : BaseItem(world, rect)
        , m_stats(std::make_shared<StatsContainer>()) { }

    /**
     * @return accessory stats
     */
    const std::shared_ptr<IStatsContainer> getStats() const override { return m_stats; }

    /**
     * @return accessory item type
     */
    ItemTypeSlot getItemTypeFromSlot() const override { return ACCESSORY; }

private:
    std::shared_ptr<StatsContainer> m_stats;
};
