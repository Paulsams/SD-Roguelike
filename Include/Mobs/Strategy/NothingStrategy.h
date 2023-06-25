#pragma once
#include "Mobs/Strategy/IMobStrategy.h"

namespace mob
{

/**
 * Nothing (dummy) strategy class
 */
class NothingStrategy : public IMobStrategy
{
public:
    /**
     * dummy
     */
    void update(Mob* mob) override {}

    /**
     * @return clone of this strategy
     */
    std::shared_ptr<IMobStrategy> clone() const override { return std::make_shared<NothingStrategy>(); }
};

}
