#pragma once

#include <memory>


/**
 * Interface for stats modificator
 */
struct IStatModificator
{
    virtual ~IStatModificator() = default;

    /**
     * @param value value
     * @return modified value
     */
    virtual float modify(float value) const = 0;

    /**
     * @return clone this stats modificator
     */
    virtual std::shared_ptr<IStatModificator> clone() const = 0;
};
