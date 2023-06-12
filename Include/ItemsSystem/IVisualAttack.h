#pragma once
#include "Utils/Common.h"

class World;

struct IVisualAttack
{
    virtual ~IVisualAttack() = default;
    virtual void draw(World* world, Vec2Int position) const = 0;
};
