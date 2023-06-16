#pragma once
#include "Mobs/Mob.h"

namespace mob {

    /**
     * Abstract factory for creating mobs of different types
     */
    class BaseMobAbstractFactory {
    public:
        /**
         * Spawns a normal mob
         * @return pointer to normal Mob
         */
        virtual Mob* createNormal(World*, int) = 0;
        /**
         * Spawns a elite mob
         * @return pointer to elite Mob
         */
        virtual Mob* createElite(World*, int) = 0;
        /**
         * Spawns a boss mob
         * @return pointer to boss Mob
         */
        virtual Mob* createBoss(World*, int) = 0;
        /**
         * Spawns a neutral mob
         * @return pointer to neutral Mob
         */
        virtual Mob* createPassive(World*, int) = 0;

        virtual ~BaseMobAbstractFactory() = default;
    };
}