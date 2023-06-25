#pragma once
#include <memory>

#include "Mobs/Factory/MobInfo.h"
#include "Mobs/Behaviour/IMobBehaviour.h"
#include "Mobs/Strategy/IMobStrategy.h"
#include "GameLoop/IUpdatable.h"
#include "WorldSystem/World.h"
#include "Stats/IHaveStats.h"
#include "WorldSystem/IVisitorEntities.h"

namespace mob
{

class IMobStrategy;

class Mob : public BaseEntity, public IUpdatable
{
public:
    static Mob* create(World* world, const MobInfo& info, const std::string& pathToBar, std::shared_ptr<StatsContainer> stats);

    Mob* clone() const;

    int getVisionRange() const;
    float getExperiencePoints() const;

    void update() override;
    void acceptVisit(std::shared_ptr<IVisitorEntities> visitor) override { visitor->visitMob(this); }
    const std::shared_ptr<IStatsContainer> getStats() const override;

    void changeBehaviour(std::shared_ptr<IMobBehaviour>);

private:
    Mob(World* world, const MobInfo& info, const std::string& pathToBar, std::shared_ptr<StatsContainer> stats);

    void subscribeOnHealthChanged();

    MobInfo m_mobInfo;
    std::shared_ptr<StatsContainer> m_stats;
    const std::string m_pathToBar;
};

}
