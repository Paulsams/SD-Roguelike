#pragma once

#include "Mobs/Strategy/BaseStateStrategy.h"


namespace mob
{

/**
 * Panic state machine mob state
 */
class Panic : public BaseStateStrategy
{
private:
    std::shared_ptr<IMobBehaviour> behaviour;

public:
    Panic() = delete;

    explicit Panic(std::shared_ptr<IMobBehaviour> startBehaviour) : behaviour(std::move(startBehaviour)) {}

    /**
     * @return clone of this Panic strategy
     */
    std::shared_ptr<BaseStateStrategy> clone() const override { return std::make_shared<Panic>(behaviour); }

    /**
     * Change behaviour of the mob
     * @param mob mob
     */
    void enable(Mob* mob) override { mob->changeBehaviour(behaviour); }

    /**
     * dummy
     */
    void disable(Mob*) override {}

    /**
     * Update strategy
     */
    void update(Mob* mob) override {}

    /**
     * @return Panic type id
     */
    size_t getTypeId() override { return typeid(Panic).hash_code(); }
};

}
