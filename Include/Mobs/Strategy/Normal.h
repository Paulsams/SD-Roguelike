#pragma once
#include "BaseStateStrategy.h"
#include "Mobs/Behaviour/AggressiveBehaviour.h"

namespace mob {

    class Normal: public BaseStateStrategy {
        std::shared_ptr<AggressiveBehaviour> aggressiveBehaviour;
    public:

        void enable() override {}
        void disable() override {}
        void update(Mob* mob) override;

    };
}