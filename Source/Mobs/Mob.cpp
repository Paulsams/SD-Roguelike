#include <utility>

#include "Stats/Modificators/BoundsModificator.h"
#include "Mobs/Mob.h"

#include "Stats/Modificators/StatWithModificators.h"

namespace mob
{
    Mob::Mob(World* world, const MobInfo& info):
        BaseEntity(world),
        m_strategy(info.strategy()),
        m_behaviour(info.startBehaviour()),
        m_stats(std::make_shared<StatsContainer>())
    {
        const auto playerHpStat = std::make_shared<StatWithModificators>(info.health);
        playerHpStat->addModificator(std::make_shared<BoundsModificator>(MinMax(0, info.health)));
        m_stats->add(Health, playerHpStat);
    }

    void Mob::update() {
        m_strategy->update(this);
        m_behaviour->update(this);
    }

    void Mob::changeBehaviour(std::shared_ptr<IMobBehaviour> newBehaviour)
    {
        m_behaviour = std::move(newBehaviour);
    }

    const std::shared_ptr<IStatsContainer> Mob::getStats() const {
        return m_stats;
    }
}
