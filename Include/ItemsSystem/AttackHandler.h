#pragma once

#include "AttackWithDamage.h"
#include "ItemsSystem/AttackSearch/IAttackSearch.h"
#include "Stats/IHaveStats.h"
#include "Utils/Direction.h"
#include "WorldSystem/BaseEntity.h"

class AttackHandler
{
public:
    explicit AttackHandler(const std::map<Vec2Int, std::shared_ptr<AttackWithDamage>>&& m_ranges)
        : m_ranges(std::move(m_ranges)) { }

    void attack(World* world, Vec2Int position, Direction direction) const
    {
        std::shared_ptr<IStat> healthStat;
        for (const auto& [range, attack] : m_ranges)
        {
            const Vec2Int endPosition = position + direction.rotate(range);
            if (attack->isPossibleAttack(world->getTileType(endPosition)))
            {
                // мейби тут делать плитки огня и подобное
                for (BaseEntity* entity : world->getEntitiesFromCell(endPosition))
                {
                    if (attack->isPossibleAttackFromEntity(entity) &&
                        attack->getSearch()->isReachable(world, position, endPosition) &&
                        entity->getStats()->tryGet(Health, healthStat))
                    {
                        healthStat->changeValueBy(attack->getDealingDamage()->getDamage(entity));
                        if (const std::shared_ptr<IVisualAttack> visualAttack = attack->getVisual())
                            visualAttack->draw(world, endPosition);
                    }
                }
            }
        }
    }
    
    bool isPossibleAttack(World* world, Vec2Int position, Vec2Int localPosition) const
    {
        const auto direction = Direction(localPosition);
        Vec2Int localRightPosition = -direction.rotate(localPosition);
        
        const auto range = m_ranges.find(localRightPosition);
        if (range != m_ranges.end())
        {
            const Vec2Int endPosition = position + localPosition;
            return range->second->isPossibleAttack(world->getTileType(endPosition)) &&
                range->second->getSearch()->isReachable(world, position, endPosition);
        }

        return false;
    }

private:
    const std::map<Vec2Int, std::shared_ptr<AttackWithDamage>> m_ranges;
};

class AttackHandlerBuilder
{
public:
    std::shared_ptr<AttackHandler> build() { return std::make_shared<AttackHandler>(std::move(m_ranges)); }
    
    AttackHandlerBuilder& addAttackData(std::vector<Vec2Int> ranges, std::shared_ptr<AttackWithDamage> attackData)
    {
        for (Vec2Int range : ranges)
            m_ranges.insert({range, attackData});
        return *this;
    }
    
private:
    std::map<Vec2Int, std::shared_ptr<AttackWithDamage>> m_ranges;
};
