#pragma once

#include "Mobs/Strategy/IMobStrategy.h"
#include "Mobs/Behaviour/IMobBehaviour.h"

#include <utility>
#include <functional>

namespace mob
{

class IMobBehaviour;
class IMobStrategy;

/**
 * Container for storing information about the mob
 */
struct MobInfo
{
    int health;
    int experiencePoints;
    int visionRange;
    std::shared_ptr<IMobStrategy> strategy;
    std::shared_ptr<IMobBehaviour> startBehaviour;

    MobInfo() = delete;

    MobInfo(int health, int experiencePoints, int visionRange, std::shared_ptr<IMobStrategy> strategy, std::shared_ptr<IMobBehaviour> startBehaviour)
        : health(health)
        , experiencePoints(experiencePoints)
        , visionRange(visionRange)
        , strategy(std::move(strategy))
        , startBehaviour(std::move(startBehaviour))
    {}

    MobInfo(const MobInfo& other) = default;
    MobInfo(MobInfo&& other) noexcept = default;

};

}
