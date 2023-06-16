#pragma once
#include "Mobs/Strategy/IState.h"
#include "Mobs/Mob.h"

namespace mob
{
/**
 * State interface for mobs
 */
class BaseStateStrategy : public IState<Mob>
{
public:
    /**
     * Update state for mob
     */
    virtual void update(Mob*) = 0;
    /**
     * Get type id for this state
     * @return
     */
    virtual size_t getTypeId() = 0;
    virtual ~BaseStateStrategy() = default;
};

}