#pragma once
#include "Mobs/Strategy/IState.h"
#include "Mobs/Mob.h"

namespace mob
{

class BaseStateStrategy : public IState<Mob>
{
public:
    virtual void update(Mob*) = 0;
    virtual size_t getTypeId() = 0;
    virtual ~BaseStateStrategy() = default;
};

}