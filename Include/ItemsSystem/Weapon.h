#pragma once

#include "AttackHandler.h"
#include "BaseItem.h"

class Weapon final : public BaseItem
{
public:
    static Weapon* create(World* world, const cocos2d::Rect& rect, ItemTypeSlot itemTypeSlot,
                          std::shared_ptr<AttackHandler> attackHandler, const std::vector<std::shared_ptr<Damage>>& damages);

    Weapon(World* world, const cocos2d::Rect& rect, ItemTypeSlot itemTypeSlot,
        std::shared_ptr<AttackHandler> attackHandler, const std::vector<std::shared_ptr<Damage>>& damages);

    void setModificatorDamage(const IDamageModificator* modificator);

    const std::shared_ptr<IStatsContainer> getStats() const override { return m_statsContainer; }
    ItemTypeSlot getItemTypeFromSlot() const override { return m_itemTypeSlot; }

    void drawIndicators(DamageIndicatorsSystems* indicators, Vec2Int position, Direction direction) const
    {
        m_attackHandler->drawIndicators(getWorld(), indicators, position, direction);
    }

    void attack(Vec2Int position, Direction direction) const
    {
        m_attackHandler->attack(getWorld(), position, direction);
    }

private:
    ItemTypeSlot m_itemTypeSlot;
    
    std::shared_ptr<AttackHandler> m_attackHandler;
    std::shared_ptr<StatsContainer> m_statsContainer;
    std::vector<std::shared_ptr<Damage>> m_damages;
};
