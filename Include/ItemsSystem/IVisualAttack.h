#pragma once
#include "Utils/Common.h"

class World;

struct IVisualAttack
{
    void draw(World* world, Vec2Int position) const;
};
