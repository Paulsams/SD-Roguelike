#pragma once
#include <map>

#include "AttackWithDamage.h"
#include "Utils/Direction.h"

class AttackHandler
{
public:
    explicit AttackHandler(const std::map<Vec2Int, std::shared_ptr<AttackWithDamage>>&& m_ranges);

    void attack(World* world, Vec2Int position, Direction direction) const;
    bool isPossibleAttack(World* world, Vec2Int position, Vec2Int localPosition) const;
    void drawIndicators(World* world, DamageIndicatorsSystems* indicators,
        Vec2Int position, Direction direction) const;

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
