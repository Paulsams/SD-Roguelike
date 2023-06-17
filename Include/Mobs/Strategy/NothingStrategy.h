#pragma once
#include "Mobs/Strategy/IMobStrategy.h"

namespace mob
{

class NothingStrategy: public IMobStrategy
{
public:
    void update(Mob* mob) override
    {

    }
};

}
