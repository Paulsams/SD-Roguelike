#include "ItemsSystem/Weapon.h"
#include "Stats/Modificators/StatWithModificators.h"

Weapon* Weapon::create(World* world, const cocos2d::Rect& rect, ItemTypeSlot itemTypeSlot,
    std::shared_ptr<AttackHandler> attackHandler, std::shared_ptr<DelegateDamage> delegateDamage,
    std::shared_ptr<IDealingDamage> damage)
{
    auto weapon = new (std::nothrow) Weapon(world, rect, itemTypeSlot, attackHandler, delegateDamage, damage);
    if (weapon && weapon->init())
    {
        weapon->autorelease();
        return weapon;
    }
    CC_SAFE_DELETE(weapon);
    return nullptr;
}

Weapon::Weapon(World* world, const cocos2d::Rect& rect, ItemTypeSlot itemTypeSlot,
    std::shared_ptr<AttackHandler> attackHandler, std::shared_ptr<DelegateDamage> delegateDamage,
    std::shared_ptr<IDealingDamage> damage)
    : BaseItem(world, rect)
    , m_itemTypeSlot(itemTypeSlot)
    , m_attackHandler(std::move(attackHandler))
    , m_statsContainer(std::make_shared<StatsContainer>())
    , m_delegateDamage(std::move(delegateDamage))
    , m_damage(damage)
{
    m_statsContainer->add(Level, std::make_shared<StatWithModificators>(0));
}
