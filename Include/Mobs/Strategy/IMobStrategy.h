#pragma once

#include <memory>

namespace mob
{

class Mob;

/**
 * Interface for mob strategy
 */
class IMobStrategy
{
public:
    /**
     * Update given mob
     */
    virtual void update(Mob* mob) = 0;

    /**
     * @return clone of this mob strategy
     */
    virtual std::shared_ptr<IMobStrategy> clone() const = 0;

    virtual ~IMobStrategy() = default;
};

}
