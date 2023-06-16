#pragma once
#include "Mobs/Strategy/IMobStrategy.h"

namespace mob
{
/**
 * Zero strategy, the mob never changes its behaviour
 */
class NothingStrategy: public IMobStrategy
{
public:
    /**
     * Update nothing strategy for mob
     * @param mob - pointer to mob
     */
    void update(Mob* mob) override
    {

    }
};

}
