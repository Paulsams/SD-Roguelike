#include <memory>

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
                if (isPossibleAttack && isReachable && entity->getStats()->tryGet(HEALTH, healthStat))
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
    const Vec2Int localRightPosition =
        direction == UP || direction == DOWN ?
        -direction.rotate(localPosition) :
        direction.rotate(localPosition);
        
    const auto range = m_ranges.find(localRightPosition);
    if (range != m_ranges.end())
    {
        const Vec2Int endPosition = position + localPosition;
        const bool isPossible = range->second->isPossibleAttack(world->getTileType(endPosition));
        const bool isReachable = range->second->getSearch()->isReachable(world, position, endPosition);
        return isPossible && isReachable;
    }

    return false;
}

// В идеале бы такое через корутины можно было сделать, но что есть уж
void AttackHandler::drawIndicators(World* world, Vec2Int position, Direction direction,
    std::function<void(DrawDamageInfo)> drawFunc) const
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
                if (isPossibleAttack && isReachable && entity->getStats()->tryGet(HEALTH, healthStat))
                {
                    damage.emplace(attack->getDamage()->get(entity));
                    break;
                }
            }
        }
        
        drawFunc({endPosition, damage.has_value() ? cocos2d::Color3B::RED :cocos2d::Color3B::GREEN, damage});
        damage.reset();
    }
}
