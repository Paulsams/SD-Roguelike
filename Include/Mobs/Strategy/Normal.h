#pragma once
#include "BaseStateStrategy.h"
#include "Mobs/Behaviour/IMobBehaviour.h"

namespace mob {

    class Normal: public BaseStateStrategy {
        std::shared_ptr<IMobBehaviour> startBehaviour;
        Normal() = delete;
    public:
        explicit Normal(std::shared_ptr<IMobBehaviour> startB) : startBehaviour(std::move(startB)) {}
        void enable(Mob* mob) override {
            mob->changeBehaviour(startBehaviour);
        }

        void disable(Mob*) override {}
        void update(Mob* mob) override {}
        size_t getTypeId() override {
            return typeid(Normal).hash_code();
        }

    };
}