#pragma once

#include "Utils/Common.h"


class World;

/**
 * Interface for attack visuals/effects
 */
struct IVisualAttack
{
    virtual ~IVisualAttack() = default;

    /**
     * Draw attack effects in the position of the world
     * @param world world
     * @param position effect position in the world
     */
    virtual void draw(World* world, Vec2Int position) const = 0;
};
