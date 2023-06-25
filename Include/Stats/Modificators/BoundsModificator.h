#pragma once

#include "Stats/Modificators/IStatModificator.h"

#include <algorithm>

/**
 * Minimum & Maximum
 */
class MinMax
{
public:
    MinMax(float min, float max)
        : m_min(min), m_max(max) { }

    /**
     * @return minimum
     */
    float getMin() const { return m_min; }

    /**
     * @return maximum
     */
    float getMax() const { return m_max; }

private:
    float m_min;
    float m_max;
};


/**
 *
 */
struct BoundsModificator : IStatModificator
{
    BoundsModificator(MinMax bounds)
        : bounds(bounds)
    {}

    /**
     * @param value absolute value
     * @return relative value in percents
     */
    float getValueFromPercent(float value) const
    {
        return (value - bounds.getMin()) / (bounds.getMax() - bounds.getMin()) * 100.0f;
    }

    /**
     * Modify value with current bounds
     * @param value value
     * @return modified value
     */
    float modify(float value) const override
    {
        return std::clamp(value, bounds.getMin(), bounds.getMax());
    }

    /**
     * @return clone of this BoundsModificator modoficator
     */
    std::shared_ptr<IStatModificator> clone() const override
    {
        return std::make_shared<BoundsModificator>(bounds);
    }

    MinMax bounds;
};
