#pragma once
#include <memory>

#include "Mobs/Factory/MobInfo.h"
#include "Mobs/Behaviour/IMobBehaviour.h"
#include "Mobs/Strategy/IMobStrategy.h"
#include "Stats/IHaveStats.h"
#include "WorldSystem/World.h"
#include "WorldSystem/IVisitorEntities.h"
#include "GameLoop/IUpdatable.h"


namespace mob
{

class IMobStrategy;

class Mob : public BaseEntity, public IUpdatable
{
public:
    /**
     * Create mob with given mob info, health bar and stats
     * @param world world
     * @param info mod info
     * @param pathToBar filesystem path to health bar
     * @param stats stats
     * @return mob
     */
    static Mob* create(World* world, const MobInfo& info, const std::string& pathToBar, std::shared_ptr<StatsContainer> stats);

    /**
     * Deep clone this mob
     * @return clone of this mob
     */
    Mob* clone() const;

    /**
     * @return vision range of this mob
     */
    int getVisionRange() const;

    /**
     * @return experience points amount given by killing this mob
     */
    float getExperiencePoints() const;

    /**
     * update this mob in the engine
     */
    void update() override;

    /**
     * Accept visitor with given function
     * @param visitor visitor
     */
    void acceptVisit(std::shared_ptr<IVisitorEntities> visitor) override { visitor->visitMob(this); }

    /**
     * @return stats of this mob
     */
    const std::shared_ptr<IStatsContainer> getStats() const override;

    /**
     * Change behaviour of this mob
     */
    void changeBehaviour(std::shared_ptr<IMobBehaviour>);

private:
    Mob(World* world, const MobInfo& info, const std::string& pathToBar, std::shared_ptr<StatsContainer> stats);

    void subscribeOnHealthChanged();

    MobInfo m_mobInfo;
    std::shared_ptr<StatsContainer> m_stats;
    const std::string m_pathToBar;
};

}
