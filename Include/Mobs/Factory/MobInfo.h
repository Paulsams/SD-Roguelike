#pragma once

#include <functional>
#include <utility>
#include "Mobs/Strategy/IMobStrategy.h"
#include "Mobs/Behaviour/IMobBehaviour.h"

namespace mob {
    class IMobBehaviour;

    class IMobStrategy;

    struct MobInfo {
        int health;
        int visionRange;
        std::shared_ptr<IMobStrategy> strategy;
        std::shared_ptr<IMobBehaviour> startBehaviour;

        MobInfo() = delete;

        MobInfo(int health, int visionRange,
                std::shared_ptr<IMobStrategy> strategy,
                std::shared_ptr<IMobBehaviour> startBehaviour)
                : health(health), visionRange(visionRange), strategy(std::move(strategy)),
                  startBehaviour(std::move(startBehaviour)) {}

        MobInfo(const MobInfo &other) : health(other.health), visionRange(other.visionRange),
                                        strategy(other.strategy),
                                        startBehaviour(other.startBehaviour) {}

        MobInfo(MobInfo &&other) noexcept : health(std::move(other.health)), visionRange(std::move(other.visionRange)),
                                   strategy(std::move(other.strategy)),
                                   startBehaviour(std::move(other.startBehaviour)) {}

    };
}