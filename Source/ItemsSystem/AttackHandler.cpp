#include <memory>

#include "ItemsSystem/AttackHandler.h"

AttackHandler::AttackHandler(const std::map<Vec2Int, std::vector<std::shared_ptr<AttackWithDamage>>>&& ranges)
    : m_ranges(std::move(ranges)) { }

void AttackHandler::attack(World* world, Vec2Int position, Direction direction) const
{
    std::shared_ptr<IStat> healthStat;
    for (const auto& [range, attacks] : m_ranges)
    {
        const Vec2Int endPosition = position + direction.rotate(range);
        for (const auto& attack : attacks)
        {
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
                        attacked(entity, damage);
                        healthStat->changeValueBy(-damage);
                        if (const std::shared_ptr<IVisualAttack> visualAttack = attack->getVisual())
                            visualAttack->draw(world, endPosition);
                        break;
                    }
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
        for (const auto& attack : range->second)
        {
            const bool isPossible = attack->isPossibleAttack(world->getTileType(endPosition));
            const bool isReachable = attack->getSearch()->isReachable(world, position, endPosition);

            if (isPossible && isReachable)
                return true;
        }

        return false;
    }

    return false;
}

// В идеале бы такое через корутины можно было сделать, но что есть уж
void AttackHandler::drawIndicatorsWithNonUnion(World* world, Vec2Int position, const std::vector<Direction>& directions,
    const std::function<void(DrawDamageInfo)>& drawFunc) const
{
    static std::set<Vec2Int> visitedDirections;
    
    std::shared_ptr<IStat> healthStat;
    std::optional<float> damage;
    for (auto [range, attacks] : m_ranges)
    {
        for (const Direction& direction : directions)
        {
            const Vec2Int endPosition = position + direction.rotate(range);
            for (const auto& attack : attacks)
            {
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

                if (visitedDirections.emplace(endPosition).second)
                    drawFunc({endPosition, damage.has_value() ? cocos2d::Color3B::RED : cocos2d::Color3B::GREEN, damage});
                damage.reset();
            }
        }
    }

    visitedDirections.clear();
}
