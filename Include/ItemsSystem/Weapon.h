#pragma once

#include "ItemsSystem/AttackHandler.h"
#include "ItemsSystem/BaseItem.h"

/**
 * Weapon class
 */
class Weapon final : public BaseItem
{
public:
    /**
     * Create weapon
     * @param world world
     * @param rect pixel rectangle in tileset
     * @param itemTypeSlot item type
     * @param attackHandler weapon attack info
     * @param damages weapon damage
     * @return weapon
     */
    static Weapon* create(World* world,
                          const cocos2d::Rect& rect,
                          ItemTypeSlot itemTypeSlot,
                          std::shared_ptr<AttackHandler> attackHandler,
                          const std::vector<std::shared_ptr<Damage>>& damages);

    /**
     * @param world world
     * @param rect pixel rectangle in tileset
     * @param itemTypeSlot item type
     * @param attackHandler weapon attack info
     * @param damages weapon damage
     */
    Weapon(World* world,
           const cocos2d::Rect& rect,
           ItemTypeSlot itemTypeSlot,
           std::shared_ptr<AttackHandler> attackHandler,
           const std::vector<std::shared_ptr<Damage>>& damages);

    /**
     * Set weapon damage modificator
     * @param modificator weapon modificator
     */
    void setModificatorDamage(const IDamageModificator* modificator);

    /**
     * @return weapon stats
     */
    const std::shared_ptr<IStatsContainer> getStats() const override { return m_statsContainer; }

    /**
     * @return weapon item type
     */
    ItemTypeSlot getItemTypeFromSlot() const override { return m_itemTypeSlot; }

    /**
     * Schedule damage indicators of the weapon from the given entity in the given directions
     * @param indicators indicators
     * @param entity entity
     * @param directions directions
     */
    void scheduleDrawIndicators(DamageIndicatorsSystems* indicators,
                                const BaseEntity* entity,
                                const std::vector<Direction>& directions) const
    {
        indicators->scheduleDraw([this, entity, &directions](const std::function<void(DrawDamageInfo)>& drawFunc)
            {
                m_attackHandler->drawIndicatorsWithNonUnion(getWorld(), entity->getPositionOnMap(), directions, drawFunc);
            });
    }

    /**
     * Perform attack with weapon from given position in given direction
     * @param position position
     * @param direction direction
     */
    void attack(Vec2Int position, Direction direction) const { m_attackHandler->attack(getWorld(), position, direction); }

    EventContainer<BaseEntity*, float> attacked; /// attacked entities event container

private:
    ItemTypeSlot m_itemTypeSlot;
    
    std::shared_ptr<AttackHandler> m_attackHandler;
    std::shared_ptr<StatsContainer> m_statsContainer;
    std::vector<std::shared_ptr<Damage>> m_damages;
};
