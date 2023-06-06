#pragma once
#include "Mobs/Strategy/IState.h"
#include "Mobs/Mob.h"

namespace mob
{

class BaseStateStrategy : public IState
{
public:
    virtual void update(Mob*) = 0;
    virtual ~BaseStateStrategy() = default;
};

}