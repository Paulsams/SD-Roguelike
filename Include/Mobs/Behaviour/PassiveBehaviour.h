#pragma once
#include "Mobs/Behaviour/IMobBehaviour.h"

namespace mob
{

class PassiveBehaviour: public IMobBehaviour
{
public:
    void update(Mob*) override;

    std::shared_ptr<IMobBehaviour> clone() const override { return std::make_shared<PassiveBehaviour>(); }
};

}
