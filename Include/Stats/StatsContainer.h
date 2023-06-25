#pragma once

#include "IStatsContainer.h"

#include <memory>
#include <unordered_map>

/**
 * Stats container class
 *
 * Aggregates stats with different types
 */
class StatsContainer final : public IStatsContainer
{
public:
    StatsContainer() = default;
    StatsContainer(const StatsContainer& other);
    StatsContainer(StatsContainer&& other) = default;

    /**
     * Try to get stat with given type from the container
     * @param type stat type
     * @param stat
     * @return true if stat with given type is presented in the container, false otherwise
     */
    bool tryGet(StatType type, std::shared_ptr<IStat>& stat) const override;

    /**
     * Add stat with given type
     * @param type stat type
     * @return true if stat was added, false otherwise
     */
    bool add(StatType type, std::shared_ptr<IStat> stat);

    /**
     * Remove stat with given type
     * @param type stat type
     * @return true if stat was removed, false otherwise
     */
    bool remove(StatType type);

    /**
     * Get stat with given type
     * @param type stat type
     * @return stat
     */
    std::shared_ptr<IStat> get(StatType type);

private:
    std::unordered_map<StatType, std::shared_ptr<IStat>> m_stats;
};
