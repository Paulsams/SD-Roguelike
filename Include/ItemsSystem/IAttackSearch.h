#pragma once

#include <Utils/Common.h>
#include <WorldSystem/World.h>

struct IAttackSearch
{
    virtual ~IAttackSearch() = default;
    virtual bool isReachable(World* world, Vec2Int startPosition, Vec2Int endPosition) const = 0;
};
