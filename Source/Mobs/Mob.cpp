#include "Stats/Modificators/BoundsModificator.h"
#include "Mobs/Mob.h"

#include <utility>


namespace mob
{

    Mob::Mob(World* world, const MobInfo& info):
        BaseEntity(world),
        m_strategy(info.strategy()),
        m_behaviour(info.startBehaviour()),
        m_statsContainer(std::make_shared<StatsContainer>())
    {
        const auto playerHpStat = std::make_shared<StatWithModificators>(info.health);
        playerHpStat->addModificator(std::make_shared<BoundsModificator>(MinMax(0, 100.0f)));
        m_statsContainer->add(Health, playerHpStat);
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
        return m_statsContainer;
    }
}
