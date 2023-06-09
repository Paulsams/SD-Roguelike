﻿#include <memory>

#include "ItemsSystem/AttackHandler.h"

AttackHandler::AttackHandler(const std::map<Vec2Int, std::shared_ptr<AttackWithDamage>>&& m_ranges)
    : m_ranges(std::move(m_ranges)) { }

void AttackHandler::attack(World* world, Vec2Int position, Direction direction) const
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
                const bool isPossibleAttack = attack->isPossibleAttackFromEntity(entity);
                const bool isReachable = attack->getSearch()->isReachable(world, position, endPosition);
                if (isPossibleAttack && isReachable && entity->getStats()->tryGet(Health, healthStat))
                {
                    float damage = attack->getDamage()->get(entity);
                    healthStat->changeValueBy(-damage);
                    if (const std::shared_ptr<IVisualAttack> visualAttack = attack->getVisual())
                        visualAttack->draw(world, endPosition);
                    break;
                }
            }
        }
    }
}

bool AttackHandler::isPossibleAttack(World* world, Vec2Int position, Vec2Int localPosition) const
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

void AttackHandler::drawIndicators(World* world, DamageIndicatorsSystems* indicators,
    Vec2Int position, Direction direction) const
{
    std::shared_ptr<IStat> healthStat;
    std::optional<float> damage;
    for (auto [range, attack] : m_ranges)
    {
        const Vec2Int endPosition = position + direction.rotate(range);
        if (attack->isPossibleAttack(world->getTileType(endPosition)))
        {
            for (BaseEntity* entity : world->getEntitiesFromCell(endPosition))
            {
                const bool isPossibleAttack = attack->isPossibleAttackFromEntity(entity);
                const bool isReachable = attack->getSearch()->isReachable(world, position, endPosition);
                if (isPossibleAttack && isReachable && entity->getStats()->tryGet(Health, healthStat))
                {
                    damage.emplace(attack->getDamage()->get(entity));
                    break;
                }
            }
        }
        
        indicators->draw(endPosition, damage.has_value() ? cocos2d::Color3B::RED :cocos2d::Color3B::GREEN, damage);
        damage.reset();
    }
}
