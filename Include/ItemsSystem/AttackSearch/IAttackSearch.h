#pragma once

#include <Utils/Common.h>
#include <WorldSystem/World.h>


/**
 * Interface for attack search algorithms
 */
struct IAttackSearch
{
    virtual ~IAttackSearch() = default;

    /**
     * Check if attack is reachable from start position to end position
     * @param world
     * @param startPosition
     * @param endPosition
     * @return true if attack can be done from startPosition to endPosition, false otherwise
     */
    virtual bool isReachable(World* world, Vec2Int startPosition, Vec2Int endPosition) const = 0;
};
