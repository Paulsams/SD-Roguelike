#pragma once

#include "AttackSearch/IAttackSearch.h"
#include "ItemsSystem/Damage.h"
#include "ItemsSystem/IVisualAttack.h"
#include "Pathfinder/Node.h"
#include "WorldSystem/FunctionVisitorEntities.h"

#include <memory>
#include <utility>


/**
 * Attack information:
 *   - Target entity/tile
 *   - Visuals/effects
 *   - AttackSearch algorithm
 */
struct AttackInfo
{
    using PossibleAttackDelegate = std::function<bool (TileType)>;
    using PossibleAttackFromEntity = std::shared_ptr<FunctionVisitorEntities<bool>>;
    
    AttackInfo(const PossibleAttackDelegate& possibleAttack,
                PossibleAttackFromEntity possibleAttackFromEntity,
                std::shared_ptr<IVisualAttack> visual,
                std::shared_ptr<IAttackSearch> search)
    : m_possibleAttack(possibleAttack)
    , m_possibleAttackFromEntity(possibleAttackFromEntity)
    , m_visual(std::move(visual))
    , m_search(std::move(search)) { }

    /**
     * @param tileType tile type
     * @return true if tile can be attacked with this, false otherwise
     */
    bool isPossibleAttack(TileType tileType) const { return m_possibleAttack(tileType); }

    /**
     * @param entity entity
     * @return true if entity can be attacked with this, false otherwise
     */
    bool isPossibleAttackFromEntity(BaseEntity* entity) const
    {
        entity->acceptVisit(m_possibleAttackFromEntity);
        return m_possibleAttackFromEntity->getReturnValue().value_or(false);
    }

    /**
     * @return attack visuals
     */
    std::shared_ptr<IVisualAttack> getVisual() const { return m_visual; }

    /**
     * @return attack search algorithm
     */
    std::shared_ptr<IAttackSearch> getSearch() const { return m_search; }
    
private:
    PossibleAttackDelegate m_possibleAttack;
    PossibleAttackFromEntity m_possibleAttackFromEntity;
    std::shared_ptr<IVisualAttack> m_visual;
    std::shared_ptr<IAttackSearch> m_search;
};


/**
 * AttackInfo + Damage
 */
struct AttackWithDamage
{
    AttackWithDamage(std::shared_ptr<AttackInfo> attackInfo, std::shared_ptr<Damage> damage)
        : m_attackInfo(std::move(attackInfo))
        , m_damage(damage) { }

    AttackWithDamage(std::shared_ptr<AttackInfo> attackInfo, Damage&& damage)
        : m_attackInfo(std::move(attackInfo))
        , m_damage(std::make_shared<Damage>(damage)) { }

    /**
     * @return attack damage
     */
    std::shared_ptr<const Damage> getDamage() const { return m_damage; }

    /**
     * @param tileType tile type
     * @return true if tile can be attacked with this, false otherwise
     */
    bool isPossibleAttack(TileType tileType) const { return m_attackInfo->isPossibleAttack(tileType); }

    /**
     * @param entity entity
     * @return true if entity can be attacked with this, false otherwise
     */
    bool isPossibleAttackFromEntity(BaseEntity* entity) const { return m_attackInfo->isPossibleAttackFromEntity(entity); }

    /**
     * @return attack visuals
     */
    std::shared_ptr<IVisualAttack> getVisual() const { return m_attackInfo->getVisual(); }

    /**
     * @return attack search algorithm
     */
    std::shared_ptr<IAttackSearch> getSearch() const { return m_attackInfo->getSearch(); }

private:
    std::shared_ptr<AttackInfo> m_attackInfo;
    std::shared_ptr<Damage> m_damage;
};
