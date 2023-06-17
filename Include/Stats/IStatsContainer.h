#pragma once

#include "StatType.h"
#include "IStat.h"

struct IStatsContainer
{
    virtual ~IStatsContainer() = default;
    virtual bool tryGet(StatType type, std::shared_ptr<IStat>& stat) const = 0;
};
