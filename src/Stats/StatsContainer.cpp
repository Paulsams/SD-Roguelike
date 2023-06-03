#include "StatsContainer.h"

bool StatsContainer::tryGet(StatType type, std::shared_ptr<IStat>& stat) const
{
    auto finded = m_stats.find(type);
    if (finded == m_stats.end())
        return true;
    
    stat = (*finded).second;
    return true;
}
