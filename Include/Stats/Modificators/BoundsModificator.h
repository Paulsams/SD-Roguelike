#pragma once
#include <algorithm>

#include "IStatModificator.h"

class MinMax
{
public:
    MinMax(float min, float max)
        : m_min(min), m_max(max) { }

    float getMin() const { return m_min; }
    float getMax() const { return m_max; }

private:
    float m_min;
    float m_max;
};

struct BoundsModificator : IStatModificator
{
    BoundsModificator(MinMax bounds)
        : bounds(bounds) { }

    float getValueFromPercent(float value) const
        { return (value - bounds.getMin()) / (bounds.getMax() - bounds.getMin()) * 100.0f; }

    float modify(float value) const override
    {
        return std::clamp(value, bounds.getMin(), bounds.getMax());
    }

    MinMax bounds;
};
