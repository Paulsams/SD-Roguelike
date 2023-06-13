#pragma once

#include "BaseItem.h"

class Accessory final : public BaseItem
{
public:
    Accessory(World* world, const cocos2d::Rect& rect)
        : BaseItem(world, rect)
        , m_stats(std::make_shared<StatsContainer>()) { }

    const std::shared_ptr<IStatsContainer> getStats() const override { return m_stats; }
    ItemTypeSlot getItemTypeFromSlot() const override { return ACCESSORY; }

private:
    std::shared_ptr<StatsContainer> m_stats;
};
