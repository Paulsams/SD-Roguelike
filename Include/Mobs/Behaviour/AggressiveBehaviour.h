#pragma once
#include <utility>

#include "ItemsSystem/AttackHandler.h"
#include "Mobs/Behaviour/IMobBehaviour.h"

namespace mob {

    /**
     * Behaviour in witch the mob attacks the player
     */
    class AggressiveBehaviour: public IMobBehaviour{
        std::shared_ptr<AttackHandler> m_attack;
    public:
        AggressiveBehaviour(std::shared_ptr<AttackHandler> attack): m_attack(std::move(attack)) {}

        /**
         * If the player is in range, then the mob runs to attack him
         * @param mob - pointer to the mob for witch the update is taking place
         */
        void update(Mob*) override;

    private:
        void drawDamageIndicators(Mob* mob) const;
    };
}