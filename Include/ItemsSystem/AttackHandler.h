#pragma once
#include <map>

#include "AttackWithDamage.h"
#include "Utils/Direction.h"

class AttackHandler
{
public:
    explicit AttackHandler(const std::map<Vec2Int, std::vector<std::shared_ptr<AttackWithDamage>>>&& ranges);

    void attack(World* world, Vec2Int position, Direction direction) const;
    bool isPossibleAttack(World* world, Vec2Int position, Vec2Int localPosition) const;
    void drawIndicatorsWithNonUnion(World* world, Vec2Int position, const std::vector<Direction>& directions,
                                    const std::function<void(DrawDamageInfo)>& drawFunc) const;

    EventContainer<BaseEntity*, float> attacked;

private:
    std::map<Vec2Int, std::vector<std::shared_ptr<AttackWithDamage>>> m_ranges;
};

class AttackHandlerBuilder
{
public:
    std::shared_ptr<AttackHandler> build() { return std::make_shared<AttackHandler>(std::move(m_ranges)); }
    
    AttackHandlerBuilder& addAttackData(std::vector<Vec2Int> ranges, std::shared_ptr<AttackWithDamage> attackData)
    {
        for (Vec2Int range : ranges)
            m_ranges[range].push_back(attackData);
        return *this;
    }
    
private:
    std::map<Vec2Int, std::vector<std::shared_ptr<AttackWithDamage>>> m_ranges;
};
