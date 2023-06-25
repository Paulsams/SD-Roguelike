#pragma once
#include "ItemsSystem/AttackHandler.h"
#include "Mobs/Behaviour/IMobBehaviour.h"

namespace mob
{

class ReplicativeBehaviour: public IMobBehaviour
{
private:
    std::shared_ptr<AttackHandler> m_attack;
public:
    ReplicativeBehaviour(std::shared_ptr<AttackHandler> attack): m_attack(attack) {}
    std::shared_ptr<IMobBehaviour> clone() const override { return std::make_shared<ReplicativeBehaviour>(m_attack); }
    void update(Mob* mob) override;
};

}