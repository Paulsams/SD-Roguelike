#pragma once
#include "Mobs/Behaviour/IMobBehaviour.h"

namespace mob {
    class PassiveBehaviour: public IMobBehaviour{
    public:
        void update(Mob*) override;
    };
}