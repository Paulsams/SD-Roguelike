#pragma once

#include <functional>
#include "Mobs/Strategy/IMobStrategy.h"
#include "Mobs/Behaviour/IMobBehaviour.h"

namespace mob {
    class IMobBehaviour;

    class IMobStrategy;

    struct MobInfo {
        int health;
        int visionRange;
        std::function<std::shared_ptr<IMobStrategy>()> strategy;
        std::function<std::shared_ptr<IMobBehaviour>()> startBehaviour;

        MobInfo() = delete;

        MobInfo(int health, int visionRange,
                const std::function<std::shared_ptr<IMobStrategy>(void)> &strategy,
                const std::function<std::shared_ptr<IMobBehaviour>(void)> &startBehaviour)
                : health(health), visionRange(visionRange), strategy(strategy),
                  startBehaviour(startBehaviour) {}

        MobInfo(const MobInfo &other) : health(other.health), visionRange(other.visionRange),
                                        strategy(other.strategy),
                                        startBehaviour(other.startBehaviour) {}

        MobInfo(MobInfo &&other) : health(std::move(other.health)), visionRange(std::move(other.visionRange)),
                                   strategy(std::move(other.strategy)),
                                   startBehaviour(std::move(other.startBehaviour)) {}

    };
}