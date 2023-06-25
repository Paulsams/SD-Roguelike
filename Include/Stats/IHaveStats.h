#pragma once

#include "Stats/StatsContainer.h"

/**
 * Interface for having stats
 */
struct IHaveStats
{
    virtual ~IHaveStats() = default;

    /**
     * @return stats
     */
    virtual const std::shared_ptr<IStatsContainer> getStats() const = 0;
};
