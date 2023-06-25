#pragma once

#include "Mobs/Mob.h"


namespace mob
{

/**
 * Mob state machine state class
 */
template <typename U>
class IState
{
public:
    /**
     * Call func entering this state on U
     */
    virtual void enable(U*) = 0;

    /**
     * Call func leaving this state on U
     */
    virtual void disable(U*) = 0;

    virtual ~IState() = default;
};

}
