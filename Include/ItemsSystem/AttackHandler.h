#pragma once

#include "AttackData.h"
#include "ItemsSystem/IAttackSearch.h"
#include "Stats/IHaveStats.h"
#include "WorldSystem/BaseEntity.h"

class AttackHandler
{
public:
    explicit AttackHandler(const std::map<Vec2Int, std::shared_ptr<AttackData>>&& m_ranges)
        : m_ranges(std::move(m_ranges)) { }

    void attack(World* world, Vec2Int position) const
    {
        std::shared_ptr<const BaseEntity> entity;
        std::shared_ptr<IStat> healthStat;
        for (const auto& [range, attack] : m_ranges)
        {
            const Vec2Int endPosition = position + range;
            if (attack->isPossibleAttack(world->getTileType(endPosition)))
            {
                // мейби партикл кинуть какой-нибудь?
                if (world->tryGetEntity(endPosition, entity) &&
                    attack->getSearch()->isReachable(world, position, endPosition) &&
                    entity->getStats()->tryGet(Health, healthStat))
                {
                    healthStat->changeValueBy(attack->getDealingDamage()->getDamage(entity));
                    attack->getVisual()->draw(world, endPosition);
                }
            }
        }
    }
    
    bool isPossibleAttack(World* world, Vec2Int position, Vec2Int localPosition) const
    {
        const auto range = m_ranges.find(localPosition);
        if (range != m_ranges.end())
        {
            const Vec2Int endPosition = position + localPosition;
            return range->second->isPossibleAttack(world->getTileType(endPosition)) &&
                range->second->getSearch()->isReachable(world, position, endPosition);
        }

        return false;
    }

private:
    const std::map<Vec2Int, std::shared_ptr<AttackData>> m_ranges;
};

class AttackHandlerBuilder
{
public:
    std::shared_ptr<AttackHandler> build() { return std::make_shared<AttackHandler>(std::move(m_ranges)); }

    template <typename T>
    AttackHandlerBuilder& addAttackData(std::vector<Vec2Int> ranges, std::shared_ptr<AttackData> attackData)
    {
        for (Vec2Int range : ranges)
            m_ranges.insert({range, attackData});
        return *this;
    }
    
private:
    std::map<Vec2Int, std::shared_ptr<AttackData>> m_ranges;
};
