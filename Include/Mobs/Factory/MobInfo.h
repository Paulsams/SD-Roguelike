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

/**
 * Mob information
 */
struct MobInfo
{
    int health; /// max health points
    float experiencePoints; /// experience points amount given by killing this mob
    int visionRange; /// vision range
    GID gid; /// pixel image id from tileset
    std::shared_ptr<IMobStrategy> strategy; /// strategy
    std::shared_ptr<IMobBehaviour> behaviour; /// behaviour

    MobInfo() = delete;

    MobInfo(const MobInfo& other);
    MobInfo(MobInfo&& other) noexcept = default;
    MobInfo(int health, float experiencePoints, int visionRange, GID gid, std::shared_ptr<IMobStrategy> strategy, std::shared_ptr<IMobBehaviour> startBehaviour);
};

}
