#pragma once

#include "IAttackSearch.h"

class AttackSearchFromStraightLines : public IAttackSearch
{
public:
    bool isReachable(World* world, Vec2Int startPosition, Vec2Int endPosition) const override
    {
        // TODO: нужно проверять прямыми линиями, что мы можем ударить, чтобы нельзя было бить через стены
        return true;
    }
};
