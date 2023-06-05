#pragma once
#include "StatsContainer.h"

struct IHaveStats
{
    virtual ~IHaveStats() = default;
    virtual const std::shared_ptr<StatsContainer> getStats() const = 0;
};
