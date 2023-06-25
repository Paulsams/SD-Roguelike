#include "Mobs/Factory/MobInfo.h"

using namespace mob;

MobInfo::MobInfo(const MobInfo& other)
        : health(other.health)
        , experiencePoints(other.experiencePoints)
        , visionRange(other.visionRange)
        , gid(other.gid)
        , strategy(other.strategy->clone())
        , behaviour(other.behaviour->clone())
{}

MobInfo::MobInfo(int health, float experiencePoints, int visionRange, GID gid, std::shared_ptr<IMobStrategy> strategy, std::shared_ptr<IMobBehaviour> startBehaviour)
        : health(health)
        , experiencePoints(experiencePoints)
        , visionRange(visionRange)
        , gid(gid)
        , strategy(std::move(strategy))
        , behaviour(std::move(startBehaviour))
{}
