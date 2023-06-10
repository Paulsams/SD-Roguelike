#pragma once
#include "Mobs/Mob.h"

namespace mob {

    class BaseMobAbstractFactory {
    public:
        virtual Mob* createNormal(World*, int) = 0;
        virtual Mob* createElite(World*, int) = 0;
        virtual Mob* createBoss(World*, int) = 0;
        virtual Mob* createPassive(World*, int) = 0;

        virtual ~BaseMobAbstractFactory() = default;
    };
}