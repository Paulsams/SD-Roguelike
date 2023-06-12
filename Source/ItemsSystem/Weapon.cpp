#include "ItemsSystem/Weapon.h"
#include "Stats/Modificators/StatWithModificators.h"

Weapon* Weapon::create(World* world, const cocos2d::Rect& rect, ItemTypeSlot itemTypeSlot,
                       std::shared_ptr<AttackHandler> attackHandler, const std::vector<std::shared_ptr<Damage>>& damages)
{
    auto weapon = new (std::nothrow) Weapon(world, rect, itemTypeSlot, attackHandler, damages);
    if (weapon && weapon->init())
    {
        weapon->autorelease();
        return weapon;
    }
    CC_SAFE_DELETE(weapon);
    return nullptr;
}

Weapon::Weapon(World* world, const cocos2d::Rect& rect, ItemTypeSlot itemTypeSlot,
    std::shared_ptr<AttackHandler> attackHandler, const std::vector<std::shared_ptr<Damage>>& damages)
    : BaseItem(world, rect)
    , m_itemTypeSlot(itemTypeSlot)
    , m_attackHandler(std::move(attackHandler))
    , m_statsContainer(std::make_shared<StatsContainer>())
    , m_damages(damages)
{
    m_statsContainer->add(Level, std::make_shared<StatWithModificators>(0));
}

void Weapon::setModificatorDamage(const IDamageModificator* modificator)
{
    for (std::shared_ptr<Damage> damage : m_damages)
        damage->changeModificator(modificator);
}
