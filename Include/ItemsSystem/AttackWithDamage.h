#pragma once
#include <memory>
#include <utility>

#include "IVisualAttack.h"
#include "AttackSearch/IAttackSearch.h"
#include "DealingDamage/IDealingDamage.h"
#include "Pathfinder/Node.h"
#include "WorldSystem/FunctionVisitorEntities.h"

struct AttackInfo
{
    using PossibleAttackDelegate = std::function<bool (TileType)>;
    using PossibleAttackFromEntity = std::shared_ptr<FunctionVisitorEntities<bool>>;
    
    AttackInfo(const PossibleAttackDelegate& possibleAttack,
        const PossibleAttackFromEntity& possibleAttackFromEntity,
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
    AttackWithDamage(std::shared_ptr<AttackInfo> attackInfo, std::shared_ptr<IDealingDamage> dealingDamage)
        : m_attackInfo(std::move(attackInfo))
        , m_dealingDamage(std::move(dealingDamage)) { }
    
    std::shared_ptr<IDealingDamage> getDealingDamage() const { return m_dealingDamage; }
    bool isPossibleAttack(TileType tileType) const { return m_attackInfo->isPossibleAttack(tileType); }
    bool isPossibleAttackFromEntity(BaseEntity* entity) const { return m_attackInfo->isPossibleAttackFromEntity(entity); }
    std::shared_ptr<IVisualAttack> getVisual() const { return m_attackInfo->getVisual(); } 
    std::shared_ptr<IAttackSearch> getSearch() const { return m_attackInfo->getSearch(); }

private:
    std::shared_ptr<AttackInfo> m_attackInfo;
    std::shared_ptr<IDealingDamage> m_dealingDamage;
};
