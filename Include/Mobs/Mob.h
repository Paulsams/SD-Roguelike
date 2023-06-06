#pragma once
#include <memory>

#include "Stats/Modificators/StatWithModificators.h"
#include "Mobs/Behaviour/IMobBehaviour.h"
#include "Mobs/Strategy/IMobStrategy.h"
#include "Mobs/Factory/MobInfo.h"
#include "ItemsSystem/AttackHandler.h"
#include "GameLoop/IUpdatable.h"
#include "WorldSystem/World.h"
#include "Stats/IHaveStats.h"

namespace mob {

    class IMobStrategy;

    class Mob : public IUpdatable, public BaseEntity {
        Mob() = delete;
    public:
        Mob(World* world, const MobInfo& info);

        void update() override;
        void changeBehaviour(std::shared_ptr<IMobBehaviour>);
        const std::shared_ptr<IStatsContainer> getStats() const override;
        int getVisionRange() const;

    private:
        int m_visionRange;
        std::shared_ptr<IMobStrategy> m_strategy;
        std::shared_ptr<IMobBehaviour> m_behaviour;
        std::shared_ptr<StatsContainer> m_statsContainer;
    };

}
