#pragma once
#include <memory>
#include <utility>

#include "Damage.h"
#include "IVisualAttack.h"
#include "AttackSearch/IAttackSearch.h"
#include "Pathfinder/Node.h"
#include "WorldSystem/FunctionVisitorEntities.h"

struct AttackInfo
{
    using PossibleAttackDelegate = std::function<bool (TileType)>;
    using PossibleAttackFromEntity = std::shared_ptr<FunctionVisitorEntities<bool>>;
    
    AttackInfo(const PossibleAttackDelegate& possibleAttack,
        PossibleAttackFromEntity possibleAttackFromEntity,
        std::shared_ptr<IVisualAttack> visualAttack, std::shared_ptr<IAttackSearch> attackSearch)
    : m_possibleAttack(possibleAttack)
    , m_possibleAttackFromEntity(possibleAttackFromEntity)
    , m_visualAttack(std::move(visualAttack))
    , m_attackSearch(std::move(attackSearch)) { }

    bool isPossibleAttack(TileType tileType) const { return m_possibleAttack(tileType); }
    bool isPossibleAttackFromEntity(BaseEntity* entity) const
        { entity->acceptVisit(m_possibleAttackFromEntity); return m_possibleAttackFromEntity->getReturnValue().value_or(false); }
    std::shared_ptr<IVisualAttack> getVisual() const { return m_visualAttack; } 
    std::shared_ptr<IAttackSearch> getSearch() const { return m_attackSearch; }
    
private:
    PossibleAttackDelegate m_possibleAttack;
    PossibleAttackFromEntity m_possibleAttackFromEntity;
    std::shared_ptr<IVisualAttack> m_visualAttack;
    std::shared_ptr<IAttackSearch> m_attackSearch;
};

struct AttackWithDamage
{
    AttackWithDamage(std::shared_ptr<AttackInfo> attackInfo, std::shared_ptr<Damage> damage)
        : m_attackInfo(std::move(attackInfo))
        , m_damage(damage) { }

    AttackWithDamage(std::shared_ptr<AttackInfo> attackInfo, Damage&& damage)
        : m_attackInfo(std::move(attackInfo))
        , m_damage(std::make_shared<Damage>(damage)) { }
    
    std::shared_ptr<const Damage> getDamage() const { return m_damage; }
    bool isPossibleAttack(TileType tileType) const { return m_attackInfo->isPossibleAttack(tileType); }
    bool isPossibleAttackFromEntity(BaseEntity* entity) const { return m_attackInfo->isPossibleAttackFromEntity(entity); }
    std::shared_ptr<IVisualAttack> getVisual() const { return m_attackInfo->getVisual(); } 
    std::shared_ptr<IAttackSearch> getSearch() const { return m_attackInfo->getSearch(); }

private:
    std::shared_ptr<AttackInfo> m_attackInfo;
    std::shared_ptr<Damage> m_damage;
};
