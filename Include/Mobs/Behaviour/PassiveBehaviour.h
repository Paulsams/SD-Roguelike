#pragma once
#include "Mobs/Behaviour/IMobBehaviour.h"

namespace mob {
    class PassiveBehaviour: public IMobBehaviour{
        std::shared_ptr<AttackHandler> m_attack;
    public:
        PassiveBehaviour(std::shared_ptr<AttackHandler> attack): m_attack(attack) {}

        void update(Mob*) override {

        }
    };
}