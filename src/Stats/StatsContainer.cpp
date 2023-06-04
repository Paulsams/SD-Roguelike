#include "StatsContainer.h"

bool StatsContainer::tryGet(StatType type, std::shared_ptr<IStat>& stat) const
{
    auto finded = m_stats.find(type);
    if (finded == m_stats.end())
        return true;
    
    stat = (*finded).second;
    return true;
}

bool StatsContainer::add(const StatType type, std::shared_ptr<IStat> stat)
{
    return m_stats.emplace(type, std::move(stat)).second;
}

bool StatsContainer::remove(const StatType type)
{
    return m_stats.erase(type) == 1;
}
