#pragma once
#include "Mobs/Strategy/IMobStrategy.h"
#include "StateContainer.h"
#include "BaseStateStrategy.h"

namespace mob
{

    class StrategyWithStates : public IMobStrategy {
        StateContainer<BaseStateStrategy, Mob> m_stateContainer;
    public:
        StrategyWithStates(StateContainer<BaseStateStrategy, Mob>&& stateContainer): m_stateContainer(std::move(stateContainer)) {}

        void update(Mob *mob) override {
            m_stateContainer.update(mob);
            m_stateContainer.get()->update(mob);
        }
    };
}
