#pragma once
#include "Mobs/Behaviour/IMobBehaviour.h"

namespace mob {

    class CowardlyBehaviour: public IMobBehaviour {
        std::shared_ptr<AttackHandler> m_attack;
    public:
        CowardlyBehaviour(std::shared_ptr<AttackHandler> attack): m_attack(attack) {}
        void update(Mob* mob) override;
    };
}