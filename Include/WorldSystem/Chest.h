#pragma once
#include "BaseEntity.h"
#include "IVisitorEntities.h"

class Chest final : public BaseEntity
{
public:
    explicit Chest(World* world)
        : BaseEntity(world)
        , m_stats(std::make_shared<StatsContainer>()) { }

    const std::shared_ptr<IStatsContainer> getStats() const override { return m_stats; }
    
    void acceptVisit(std::shared_ptr<IVisitorEntities> visitor) override { visitor->visitChest(this); }

private:
    std::shared_ptr<StatsContainer> m_stats;
};
