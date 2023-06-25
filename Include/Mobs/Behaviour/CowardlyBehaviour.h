#pragma once

#include "ItemsSystem/AttackHandler.h"
#include "Mobs/Behaviour/IMobBehaviour.h"


namespace mob
{

/**
 * Cowardly behaviour class
 *
 * Run from the player and attack if possible
 */
class CowardlyBehaviour: public IMobBehaviour
{
private:
    std::shared_ptr<AttackHandler> m_attack;
public:
    CowardlyBehaviour(std::shared_ptr<AttackHandler> attack): m_attack(attack) {}

    /**
     * update mob with this behaviour
     */
    void update(Mob* mob) override;

    /**
     * @return clone of this behaviour
     */
    std::shared_ptr<IMobBehaviour> clone() const override { return std::make_shared<CowardlyBehaviour>(m_attack); }
};

}