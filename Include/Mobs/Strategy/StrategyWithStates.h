#pragma once
#include "Mobs/Strategy/IMobStrategy.h"
#include "StateMachine.h"
#include "BaseStateStrategy.h"

namespace mob
{
    /**
     * Strategy with states for mobs
     */
    class StrategyWithStates : public IMobStrategy {
        StateMachine<BaseStateStrategy, Mob> m_stateContainer;
        StrategyWithStates() = delete;
    public:

        StrategyWithStates(StateMachine<BaseStateStrategy, Mob>&& stateContainer): m_stateContainer(std::move(stateContainer)) {}
        StrategyWithStates(const StateMachine<BaseStateStrategy, Mob>& stateContainer): m_stateContainer(stateContainer) {}
        /**
         * Update strategy
         * @param mob - pointer to mob
         */
        void update(Mob *mob) override {
            m_stateContainer.update(mob);
            m_stateContainer.get()->update(mob);
        }
    };
}
