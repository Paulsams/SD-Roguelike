#pragma once

#include "Mobs/Factory/BaseMobAbstractFactory.h"
#include "Mobs/Factory/MobsConfig.h"
#include "WorldSystem/World.h"

namespace mob {

    class MobsFactoryFromConfig: public BaseMobAbstractFactory {
        MobsConfig m_mobsConfig;
        World* world;
    public:

        Mob createNormal(int gid) override {
            return Mob(world, m_mobsConfig.normals.at(gid));
        }
        Mob createElite(int gid) override {
            return Mob(world, m_mobsConfig.elites.at(gid));
        }
        Mob createBoss(int gid) override {
            return Mob(world, m_mobsConfig.bosses.at(gid));
        }
    };
}