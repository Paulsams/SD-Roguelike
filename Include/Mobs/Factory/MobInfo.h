#pragma once

#include "Mobs/Strategy/IMobStrategy.h"
#include "Mobs/Behaviour/IMobBehaviour.h"

#include <utility>
#include <functional>

namespace mob
{

class IMobBehaviour;
class IMobStrategy;

struct MobInfo
{
    int health;
    int visionRange;
    std::shared_ptr<IMobStrategy> strategy;
    std::shared_ptr<IMobBehaviour> startBehaviour;

    MobInfo() = delete;

    MobInfo(int health, int visionRange, std::shared_ptr<IMobStrategy> strategy, std::shared_ptr<IMobBehaviour> startBehaviour)
        : health(health)
        , visionRange(visionRange)
        , strategy(std::move(strategy))
        , startBehaviour(std::move(startBehaviour))
    {}

    MobInfo(const MobInfo& other) = default;
    MobInfo(MobInfo&& other) noexcept = default;

//    MobInfo(MobInfo&& other) noexcept
//        : health(other.health)
//        , visionRange(other.visionRange)
//        , strategy(std::move(other.strategy))
//        , startBehaviour(std::move(other.startBehaviour))
//    {}

};

}
