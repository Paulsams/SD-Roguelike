#pragma once
#include "Mobs/Mob.h"

namespace mob
{

class IState
{
public:
    virtual void enable(Mob*) = 0;
    virtual void disable(Mob*) = 0;
    virtual ~IState() = default;
};

}
