#pragma once

#include "Mobs/Strategy/BaseStateStrategy.h"
#include "Mobs/Behaviour/IMobBehaviour.h"


namespace mob {

/**
 * Normal state class
 */
class Normal : public BaseStateStrategy
{
private:
    std::shared_ptr<IMobBehaviour> behaviour;
    Normal() = delete;

public:
    explicit Normal(std::shared_ptr<IMobBehaviour> startBehaviour) : behaviour(std::move(startBehaviour)) {}

    /**
     * @return clone of this normal state
     */
    std::shared_ptr<BaseStateStrategy> clone() const override { return std::make_shared<Normal>(behaviour); }

    /**
     * Change behaviour of the given mob
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
     * @return Normal type id
     */
    size_t getTypeId() override { return typeid(Normal).hash_code(); }
};

}
