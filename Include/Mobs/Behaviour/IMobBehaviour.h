#pragma once
#include "Mobs/Mob.h"

namespace mob {

    class Mob;

    class IMobBehaviour {
    public:
        virtual ~IMobBehaviour() = default;
        virtual void update(Mob* mob) = 0;
    };

}