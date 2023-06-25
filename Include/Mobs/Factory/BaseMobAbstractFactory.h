#pragma once

#include "Mobs/Mob.h"


namespace mob {

/**
 * Interface for mob factory
 */
class BaseMobAbstractFactory
{
public:
    /**
     * @return normal mob with given GID
     */
    virtual Mob* createNormal(World*, int) = 0;

    /**
     * @return elite mob with given GID
     */
    virtual Mob* createElite(World*, int) = 0;

    /**
     * @return boss mob with given GID
     */
    virtual Mob* createBoss(World*, int) = 0;

    /**
     * @return passive mob with given GID
     */
    virtual Mob* createPassive(World*, int) = 0;

    virtual ~BaseMobAbstractFactory() = default;
};

}