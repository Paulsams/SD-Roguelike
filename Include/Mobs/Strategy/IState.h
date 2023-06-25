#pragma once
#include "Mobs/Mob.h"

namespace mob
{

template <typename U>
class IState
{
public:
    virtual void enable(U*) = 0;
    virtual void disable(U*) = 0;
    virtual ~IState() = default;
};

}
