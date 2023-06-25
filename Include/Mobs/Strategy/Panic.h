#pragma once

#include "Mobs/Strategy/BaseStateStrategy.h"

namespace mob
{

class Panic : public BaseStateStrategy
{
private:
    std::shared_ptr<IMobBehaviour> behaviour;

public:
    Panic() = delete;

    explicit Panic(std::shared_ptr<IMobBehaviour> startBehaviour) : behaviour(std::move(startBehaviour)) {}

    std::shared_ptr<BaseStateStrategy> clone() const override { return std::make_shared<Panic>(behaviour); }

    void enable(Mob* mob) override { mob->changeBehaviour(behaviour); }

    void disable(Mob*) override {}
    void update(Mob* mob) override {}

    size_t getTypeId() override { return typeid(Panic).hash_code(); }
};

}
