#pragma once
#include "Mobs/Strategy/IMobStrategy.h"
#include "StateMachine.h"
#include "BaseStateStrategy.h"

namespace mob
{

    class StrategyWithStates : public IMobStrategy {
        StateMachine<BaseStateStrategy, Mob> m_stateContainer;
    public:
        StrategyWithStates(StateMachine<BaseStateStrategy, Mob>&& stateContainer): m_stateContainer(std::move(stateContainer)) {}

        void update(Mob *mob) override {
            m_stateContainer.update(mob);
            m_stateContainer.get()->update(mob);
        }
    };
}
