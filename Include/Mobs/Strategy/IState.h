#pragma once
#include "Mobs/Mob.h"

namespace mob
{
    /**
     * State interface for state machine
     * @tparam U - the type for withc the update is being perfomed
     */
    template <typename U>
    class IState
    {
    public:
        /**
         * Do something when entering this state
         */
        virtual void enable(U*) = 0;
        /**
         * Do something in transition from this state
         */
        virtual void disable(U*) = 0;
        virtual ~IState() = default;
    };

}
