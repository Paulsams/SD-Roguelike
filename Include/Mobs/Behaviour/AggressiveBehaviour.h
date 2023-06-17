#pragma once
#include <utility>

#include "ItemsSystem/AttackHandler.h"
#include "Mobs/Behaviour/IMobBehaviour.h"

namespace mob {

    class AggressiveBehaviour: public IMobBehaviour{
        std::shared_ptr<AttackHandler> m_attack;
    public:
        AggressiveBehaviour(std::shared_ptr<AttackHandler> attack): m_attack(std::move(attack)) {}
        
        void update(Mob*) override;

    private:
        void drawDamageIndicators(Mob* mob) const;
    };
}