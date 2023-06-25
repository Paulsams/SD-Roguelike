#pragma once

#include <memory>

namespace mob
{

class Mob;

class IMobBehaviour
{
public:
    virtual ~IMobBehaviour() = default;
    virtual std::shared_ptr<IMobBehaviour> clone() const = 0;
    virtual void update(Mob* mob) = 0;
};

}