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

    void enable(Mob* mob) override
    {
        mob->changeBehaviour(startBehaviour);
    }

    void disable(Mob*) override {}
    void update(Mob* mob) override {}

    size_t getTypeId() override
    {
        return typeid(Panic).hash_code();
    }
};

}
