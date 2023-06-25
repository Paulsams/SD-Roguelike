#pragma once

#include "Mobs/Strategy/IState.h"
#include "Mobs/Mob.h"


namespace mob
{

/**
 * Interface for mob state strategy
 */
class BaseStateStrategy : public IState<Mob>
{
public:
    /**
     * update given mob
     */
    virtual void update(Mob*) = 0;

    /**
     * @return type id
     */
    virtual size_t getTypeId() = 0;

    /**
     * @return clone of this state strategy
     */
    virtual std::shared_ptr<BaseStateStrategy> clone() const = 0;

    virtual ~BaseStateStrategy() = default;
};

}