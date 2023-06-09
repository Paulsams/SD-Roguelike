#pragma once
#include <memory>

#include "Factory/MobInfo.h"
#include "GameLoop/IUpdatable.h"
#include "WorldSystem/BaseEntity.h"
#include "WorldSystem/IVisitorEntities.h"

namespace mob {

    class IMobStrategy;

    class Mob : public IUpdatable, public BaseEntity {
        void update() override;
        void changeBehaviour(std::shared_ptr<IMobBehaviour>);
        const std::shared_ptr<IStatsContainer> getStats() const override;
        
        void acceptVisit(std::shared_ptr<IVisitorEntities> visitor) override { visitor->visitMob(this); }

    private:
        Mob(World* world, const MobInfo& info);
        
        std::shared_ptr<IMobStrategy> m_strategy;
        std::shared_ptr<IMobBehaviour> m_behaviour;
        std::shared_ptr<StatsContainer> m_stats;
    };

}
