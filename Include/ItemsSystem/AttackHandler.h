#pragma once
#include <map>

#include "AttackWithDamage.h"
#include "Utils/Direction.h"


/**
 * Attack information:
 *   - damage
 *   - range
 */
class AttackHandler
{
public:
    /**
     * @param ranges of the attack
     */
    explicit AttackHandler(const std::map<Vec2Int, std::vector<std::shared_ptr<AttackWithDamage>>>&& ranges);

    /**
     * Perform attack from position in the given direction considering attack ranges
     * @param world world
     * @param position position
     * @param direction direction
     */
    void attack(World* world, Vec2Int position, Direction direction) const;

    /**
     * Check if attack on the given localPosition from position can be performed
     * @param world world
     * @param position position
     * @param localPosition local position
     * @return true if attack is possible, false otherwise
     */
    bool isPossibleAttack(World* world, Vec2Int position, Vec2Int localPosition) const;

    /**
     * Draw damage indicators in the world with given draw function
     * @param world world
     * @param position position
     * @param directions directions
     * @param drawFunc draw function
     */
    void drawIndicatorsWithNonUnion(World* world, Vec2Int position, const std::vector<Direction>& directions,
                                    const std::function<void(DrawDamageInfo)>& drawFunc) const;

    EventContainer<BaseEntity*, float> attacked; /// attacked entities event container

private:
    std::map<Vec2Int, std::vector<std::shared_ptr<AttackWithDamage>>> m_ranges;
};



/**
 * Attack handler builder
 */
class AttackHandlerBuilder
{
public:
    /**
     * @return built AttackHandler with ranges provided before
     */
    std::shared_ptr<AttackHandler> build() { return std::make_shared<AttackHandler>(std::move(m_ranges)); }

    /**
     * Add attack data for building AttackHandler
     * @param ranges attack ranges
     * @param attackData attack info + damage
     * @return this builder
     */
    AttackHandlerBuilder& addAttackData(std::vector<Vec2Int> ranges, std::shared_ptr<AttackWithDamage> attackData)
    {
        for (Vec2Int range : ranges)
            m_ranges[range].push_back(attackData);
        return *this;
    }
    
private:
    std::map<Vec2Int, std::vector<std::shared_ptr<AttackWithDamage>>> m_ranges;
};
