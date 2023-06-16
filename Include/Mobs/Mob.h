#pragma once
#include <memory>

#include "Mobs/Behaviour/IMobBehaviour.h"
#include "Mobs/Strategy/IMobStrategy.h"
#include "Mobs/Factory/MobInfo.h"
#include "GameLoop/IUpdatable.h"
#include "WorldSystem/World.h"
#include "Stats/IHaveStats.h"
#include "WorldSystem/IVisitorEntities.h"

namespace mob {

    class IMobStrategy;

    class Mob : public BaseEntity, public IUpdatable {
    public:
        static Mob* create(World* world, cocos2d::Sprite* sprite, const MobInfo& info);

        /**
         * Update this mob
         */
        void update() override;
        /**
         * Change behaviour to new
         */
        void changeBehaviour(std::shared_ptr<IMobBehaviour>);
        /**
         * Get mob stats
         */
        const std::shared_ptr<IStatsContainer> getStats() const override;
        /**
         * Get mob vision range
         */
        int getVisionRange() const;
        /**
         * Get mob experience points
         */
        float getExperiencePoints() const;
        /**
         * Visit mob
         */
        void acceptVisit(std::shared_ptr<IVisitorEntities> visitor) override { visitor->visitMob(this); }

    private:

        Mob(World* world, const MobInfo& info);
        
        float m_experiencePoints;
        int m_visionRange;
        std::shared_ptr<IMobStrategy> m_strategy;
        std::shared_ptr<IMobBehaviour> m_behaviour;
        std::shared_ptr<StatsContainer> m_stats;
    };

}
