#pragma once

#include "IAttackSearch.h"


/**
 * Check if attack is reachable by straight lines from attacker
 */
class AttackSearchFromStraightLines : public IAttackSearch
{
public:
    bool isReachable(World* world, Vec2Int startPosition, Vec2Int endPosition) const override
    {
        // TODO: нужно проверять прямыми линиями, что мы можем ударить, чтобы нельзя было бить через стены
        return true;
    }
};
