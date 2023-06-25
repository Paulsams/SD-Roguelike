#pragma once

#include "Stats/StatType.h"
#include "Stats/IStat.h"


/**
 * Stats container
 */
struct IStatsContainer
{
    virtual ~IStatsContainer() = default;

    /**
     * Try to get stat from container
     * @param type stat type
     * @param outStat out stat
     * @return true if stat type presents in container and outStat was set with it, false otherwise
     */
    virtual bool tryGet(StatType type, std::shared_ptr<IStat>& outStat) const = 0;
};
