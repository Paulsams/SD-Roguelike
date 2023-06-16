#pragma once

#include "Mobs/Strategy/BaseStateStrategy.h"

namespace mob
{

class Panic : public BaseStateStrategy
{
private:
    std::shared_ptr<IMobBehaviour> startBehaviour;

public:
    Panic() = delete;

    explicit Panic(std::shared_ptr<IMobBehaviour> startB) : startBehaviour(std::move(startB)) {}

    /**
    * Changes behaviour to start for mob
    * @param mob - pointer to mob
    */
    void enable(Mob* mob) override
    {
        mob->changeBehaviour(startBehaviour);
    }

    void disable(Mob*) override {}
    /**
    * Update state for mob
    */
    void update(Mob* mob) override {}

    /**
     * Get type id of panic state
     * @return type id of panic state
     */
    size_t getTypeId() override
    {
        return typeid(Panic).hash_code();
    }
};

}
