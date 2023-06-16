#pragma once
#include "ItemsSystem/AttackHandler.h"
#include "Mobs/Behaviour/IMobBehaviour.h"

namespace mob {

    /**
     * Behaviour in witch the mob starts to run away at the sight of the player
     */
    class CowardlyBehaviour: public IMobBehaviour {
        std::shared_ptr<AttackHandler> m_attack;
    public:
        CowardlyBehaviour(std::shared_ptr<AttackHandler> attack): m_attack(attack) {}
        /**
         *  If the player is in range, then the mob runs away
         * @param mob - pointer to the mob for witch the update is taking place
         */
        void update(Mob* mob) override;
    };
}