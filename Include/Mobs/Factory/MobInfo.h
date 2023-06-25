#pragma once

#include "Mobs/Strategy/IMobStrategy.h"
#include "Mobs/Behaviour/IMobBehaviour.h"
#include "Utils/Common.h"

#include <utility>
#include <functional>

namespace mob
{

class IMobBehaviour;
class IMobStrategy;

struct MobInfo
{
    int health;
    float experiencePoints;
    int visionRange;
    GID gid;
    std::shared_ptr<IMobStrategy> strategy;
    std::shared_ptr<IMobBehaviour> behaviour;

    MobInfo() = delete;

    MobInfo(const MobInfo& other);
    MobInfo(MobInfo&& other) noexcept = default;
    MobInfo(int health, float experiencePoints, int visionRange, GID gid, std::shared_ptr<IMobStrategy> strategy, std::shared_ptr<IMobBehaviour> startBehaviour);
};

}
