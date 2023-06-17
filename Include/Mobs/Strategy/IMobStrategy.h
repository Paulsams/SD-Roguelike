#pragma once
#include "Mobs/Mob.h"

namespace mob
{

class Mob;

class IMobStrategy
{
public:
    virtual void update(Mob* mob) = 0;

    virtual ~IMobStrategy() = default;
};

}
