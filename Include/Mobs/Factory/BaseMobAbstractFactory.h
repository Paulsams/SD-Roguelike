#pragma once
#include "Mobs/Mob.h"

namespace mob {

    class BaseMobAbstractFactory {
    public:
        virtual Mob createNormal(int) = 0;
        virtual Mob createElite(int) = 0;
        virtual Mob createBoss(int) = 0;

        virtual ~BaseMobAbstractFactory() = default;
    };
}