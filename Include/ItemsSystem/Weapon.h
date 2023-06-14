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

    void scheduleDrawIndicators(DamageIndicatorsSystems* indicators, const BaseEntity* entity,
        const std::vector<Direction>& directions) const
    {
        indicators->scheduleDraw([this, entity, &directions](const std::function<void(DrawDamageInfo)>& drawFunc)
        {
            m_attackHandler->drawIndicatorsWithNonUnion(getWorld(), entity->getPositionOnMap(), directions, drawFunc);
        });
    }

    void attack(Vec2Int position, Direction direction) const
    {
        m_attackHandler->attack(getWorld(), position, direction);
    }

    EventContainer<BaseEntity*, float> attacked; 

private:
    ItemTypeSlot m_itemTypeSlot;
    
    std::shared_ptr<AttackHandler> m_attackHandler;
    std::shared_ptr<StatsContainer> m_statsContainer;
    std::vector<std::shared_ptr<Damage>> m_damages;
};
