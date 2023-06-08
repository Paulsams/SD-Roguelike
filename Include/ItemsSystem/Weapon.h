#pragma once
#include <utility>

#include "AttackHandler.h"
#include "BaseItem.h"
#include "DealingDamage/DelegateDamage.h"
#include "Stats/Modificators/StatWithModificators.h"

class Weapon final : public BaseItem
{
public:
    static Weapon* create(World* world, const cocos2d::Rect& rect, ItemTypeSlot itemTypeSlot,
        std::shared_ptr<AttackHandler> attackHandler, std::shared_ptr<DelegateDamage> delegateDamage)
    {
        auto weapon = new (std::nothrow) Weapon(world, rect, itemTypeSlot, attackHandler, delegateDamage);
        if (weapon && weapon->init())
        {
            weapon->autorelease();
            return weapon;
        }
        CC_SAFE_DELETE(weapon);
        return nullptr;
    }
    
    Weapon(World* world, const cocos2d::Rect& rect, ItemTypeSlot itemTypeSlot, std::shared_ptr<AttackHandler> attackHandler,
            std::shared_ptr<DelegateDamage> delegateDamage)
        : BaseItem(world, rect)
        , m_itemTypeSlot(itemTypeSlot)
        , m_attackHandler(std::move(attackHandler))
        , m_statsContainer(std::make_shared<StatsContainer>())
        , m_delegateDamage(std::move(delegateDamage))
    {
        m_statsContainer->add(Level, std::make_shared<StatWithModificators>(0));
    }

    void setDealingDamage(std::shared_ptr<IDealingDamage> dealingDamage) const
        { m_delegateDamage->setDealingDamage(dealingDamage); }

    const std::shared_ptr<IStatsContainer> getStats() const override { return m_statsContainer; }
    ItemTypeSlot getItemTypeFromSlot() const override { return m_itemTypeSlot; }

private:
    ItemTypeSlot m_itemTypeSlot;
    
    std::shared_ptr<AttackHandler> m_attackHandler;
    std::shared_ptr<StatsContainer> m_statsContainer;
    std::shared_ptr<DelegateDamage> m_delegateDamage;
};
