#pragma once
#include "Mobs/Behaviour/IMobBehaviour.h"

namespace mob {
    /**
     * Behaviour in witch the mob does not respond to the player
     */
    class PassiveBehaviour: public IMobBehaviour{
    public:
        /*
         * Random walking
         * @param pointer to the mob for witch the update is taking place
         */
        void update(Mob*) override;
    };
}