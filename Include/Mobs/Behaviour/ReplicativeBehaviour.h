#pragma once

#include "ItemsSystem/AttackHandler.h"
#include "Mobs/Behaviour/IMobBehaviour.h"


namespace mob
{

/**
 * Replicative behaviour class
 *
 * Run from the player and attack him if possible
 *
 * Try to replicate every step,
 * if successful -- replicate and change behaviour to aggressive, else stay in this behaviour
 */
class ReplicativeBehaviour: public IMobBehaviour
{
private:
    std::shared_ptr<AttackHandler> m_attack;
public:
    ReplicativeBehaviour(std::shared_ptr<AttackHandler> attack): m_attack(attack) {}

    /**
     * update mob with this behaviour
     */
    void update(Mob* mob) override;

    /**
     * @return clone of this behaviour
     */
    std::shared_ptr<IMobBehaviour> clone() const override { return std::make_shared<ReplicativeBehaviour>(m_attack); }
};

}