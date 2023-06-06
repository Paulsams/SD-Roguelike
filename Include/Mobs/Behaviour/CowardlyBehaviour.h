#pragma once
#include "Mobs/Behaviour/IMobBehaviour.h"

namespace mob {

    class CowardlyBehaviour: public IMobBehaviour {
    public:
        void update(Mob* mob) override {
            const Player* player = mob->getWorld()->getNearestPlayer(mob->getPositionInWorld());
        }
    };
}