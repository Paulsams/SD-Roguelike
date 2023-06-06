#pragma once

#include <memory>
#include <unordered_map>
#include "IStatsContainer.h"

class StatsContainer final : public IStatsContainer
{
public:
    bool tryGet(StatType type, std::shared_ptr<IStat>& stat) const override;
    bool add(StatType type, std::shared_ptr<IStat> stat);
    bool remove(StatType type);

    std::shared_ptr<IStat> get(StatType type);
    
private:
    std::unordered_map<StatType, std::shared_ptr<IStat>> m_stats;
};
