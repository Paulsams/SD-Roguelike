#pragma once
#include "BaseStateStrategy.h"
#include "Mobs/Behaviour/IMobBehaviour.h"

namespace mob {

class Normal : public BaseStateStrategy
{
private:
    std::shared_ptr<IMobBehaviour> behaviour;
    Normal() = delete;

public:
    explicit Normal(std::shared_ptr<IMobBehaviour> startBehaviour) : behaviour(std::move(startBehaviour)) {}

    std::shared_ptr<BaseStateStrategy> clone() const override { return std::make_shared<Normal>(behaviour); }

    void enable(Mob* mob) override { mob->changeBehaviour(behaviour); }

    void disable(Mob*) override {}
    void update(Mob* mob) override {}
    size_t getTypeId() override { return typeid(Normal).hash_code(); }
};
}