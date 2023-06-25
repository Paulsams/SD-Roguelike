#pragma once

#include "ItemsSystem/AttackHandler.h"
#include "Mobs/Behaviour/IMobBehaviour.h"

#include <utility>


namespace mob
{

/**
 * Aggressive behaviour class
 *
 * Follow player and attack him if possible
 */
class AggressiveBehaviour: public IMobBehaviour
{
private:
    std::shared_ptr<AttackHandler> m_attack;

public:
    AggressiveBehaviour(std::shared_ptr<AttackHandler> attack): m_attack(std::move(attack)) {}

    /**
     * update mob with this behaviour
     */
    void update(Mob*) override;

    /**
     * @return clone of this behaviour
     */
    std::shared_ptr<IMobBehaviour> clone() const override { return std::make_shared<AggressiveBehaviour>(m_attack); }

private:
    void drawDamageIndicators(Mob* mob) const;
};

}