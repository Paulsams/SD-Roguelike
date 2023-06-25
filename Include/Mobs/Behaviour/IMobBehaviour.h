#pragma once

#include <memory>


namespace mob
{

class Mob;

class IMobBehaviour
{
public:
    virtual ~IMobBehaviour() = default;

    /**
     * update mob with this behaviour
     */
    virtual void update(Mob* mob) = 0;

    /**
     * @return clone of this behaviour
     */
    virtual std::shared_ptr<IMobBehaviour> clone() const = 0;
};

}