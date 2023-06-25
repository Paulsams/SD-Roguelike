#pragma once

#include "Mobs/Factory/BaseMobAbstractFactory.h"
#include "Mobs/Factory/MobsConfig.h"
#include "UI/StatBar.h"
#include "Utils/Common.h"

namespace mob {

    class MobsFactoryFromConfig final : public BaseMobAbstractFactory {
        std::shared_ptr<MobsConfig> m_mobsConfig;
    public:
        explicit MobsFactoryFromConfig(std::shared_ptr<MobsConfig> mobsConfig)
            : m_mobsConfig(mobsConfig) { }
        
        Mob* createNormal(World* world, GID gid) override
        {
            return createMob(world, gid, m_mobsConfig->normals, Paths::toHealthBar);
        }

        Mob* createElite(World* world, GID gid) override
        {
            return createMob(world, gid, m_mobsConfig->elites, Paths::toHealthBar);
        }

        Mob* createBoss(World* world, GID gid) override
        {
            return createMob(world, gid, m_mobsConfig->bosses, Paths::toHealthBar);
        }

        Mob* createPassive(World* world, GID gid) override
        {
            return createMob(world, gid, m_mobsConfig->passives, Paths::toExperiencePointsBar);
        }

    private:
        Mob* createMob(World* world, GID gid, const std::unordered_map<int, MobInfo>& createMap,
            const std::string& pathToBar)
        {
            const MobInfo& info = createMap.at(gid);
            auto stats = std::make_shared<StatsContainer>();
            const auto mobHpStat = std::make_shared<StatWithModificators>(info.health);
            mobHpStat->addModificator(std::make_shared<BoundsModificator>(MinMax(0, info.health)));
            stats->add(HEALTH, mobHpStat);

            Mob* mob = Mob::create(world, info, pathToBar, stats);
            return mob;
        }
    };
}
