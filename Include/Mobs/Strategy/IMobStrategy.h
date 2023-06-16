#pragma once
#include "Mobs/Mob.h"

namespace mob
{

class Mob;

/**
 * Strategy interface for mob
 */
class IMobStrategy
{
public:
    /**
     * Update current state
     * @param mob
     */
    virtual void update(Mob* mob) = 0;

    virtual ~IMobStrategy() = default;
};

}
