#pragma once

#include "AttackHandler.h"
#include "BaseItem.h"
#include "DealingDamage/DelegateDamage.h"

class Weapon final : public BaseItem, public IDealingDamage
{
public:
    static Weapon* create(World* world, const cocos2d::Rect& rect, ItemTypeSlot itemTypeSlot,
        std::shared_ptr<AttackHandler> attackHandler, std::shared_ptr<DelegateDamage> delegateDamage,
        std::shared_ptr<IDealingDamage> damage);

    Weapon(World* world, const cocos2d::Rect& rect, ItemTypeSlot itemTypeSlot, std::shared_ptr<AttackHandler> attackHandler,
           std::shared_ptr<DelegateDamage> delegateDamage, std::shared_ptr<IDealingDamage> damage);

    void setDealingDamage(std::shared_ptr<IDealingDamage> dealingDamage) const
        { m_delegateDamage->setDealingDamage(dealingDamage); }

    const std::shared_ptr<IStatsContainer> getStats() const override { return m_statsContainer; }
    ItemTypeSlot getItemTypeFromSlot() const override { return m_itemTypeSlot; }

    void drawIndicators(DamageIndicatorsSystems* indicators, Vec2Int position, Direction direction) const
    {
        m_attackHandler->drawIndicators(indicators, position, direction);
    }

    void attack(Vec2Int position, Direction direction) const
    {
        m_attackHandler->attack(getWorld(), position, direction);
    }

    float getDamage(const BaseEntity* otherEntity) const override { return m_damage->getDamage(otherEntity); }

private:
    ItemTypeSlot m_itemTypeSlot;
    
    std::shared_ptr<AttackHandler> m_attackHandler;
    std::shared_ptr<StatsContainer> m_statsContainer;
    std::shared_ptr<DelegateDamage> m_delegateDamage;
    std::shared_ptr<IDealingDamage> m_damage;
};
