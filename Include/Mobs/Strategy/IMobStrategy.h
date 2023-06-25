#pragma once

#include <memory>

namespace mob
{

class Mob;

class IMobStrategy
{
public:
    virtual void update(Mob* mob) = 0;
    virtual std::shared_ptr<IMobStrategy> clone() const = 0;
    virtual ~IMobStrategy() = default;
};

}
