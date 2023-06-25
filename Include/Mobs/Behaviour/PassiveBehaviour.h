#pragma once

#include "Mobs/Behaviour/IMobBehaviour.h"


namespace mob
{

/**
 * Passive behaviour class
 *
 * Move in the random directions and ignore player
 */
class PassiveBehaviour: public IMobBehaviour
{
public:
    /**
     * update mob with this behaviour
     */
    void update(Mob*) override;

    /**
     * @return clone of this behaviour
     */
    std::shared_ptr<IMobBehaviour> clone() const override { return std::make_shared<PassiveBehaviour>(); }
};

}
