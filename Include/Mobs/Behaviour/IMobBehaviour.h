#pragma once
#include "Mobs/Mob.h"

namespace mob {

    class Mob;

    /**
     * Interface that defines the behaviour of the mob
     */
    class IMobBehaviour {
    public:
        virtual ~IMobBehaviour() = default;

        /**
         * Update mob actions based on behaviour
         * @param mob - pointer to the mob for witch the update is taking place
         */
        virtual void update(Mob* mob) = 0;
    };

}