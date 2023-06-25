#pragma once

#include "Mobs/Strategy/IMobStrategy.h"
#include "Mobs/Strategy/StateMachine.h"
#include "Mobs/Strategy/BaseStateStrategy.h"


namespace mob
{

/**
 * Mob strategy with state machine
 */
class StrategyWithStates : public IMobStrategy
{
private:
    StateMachine<BaseStateStrategy, Mob> m_stateContainer;
    StrategyWithStates() = delete;

public:
    StrategyWithStates(StateMachine<BaseStateStrategy, Mob>&& stateContainer) noexcept
        : m_stateContainer(std::move(stateContainer))
    {}

    StrategyWithStates(const StateMachine<BaseStateStrategy, Mob>& stateContainer)
        : m_stateContainer(stateContainer)
    {}

    /**
     * @return clone of this strategy
     */
    std::shared_ptr<IMobStrategy> clone() const override { return std::make_shared<StrategyWithStates>(m_stateContainer); }

    /**
     * Update strategy
     */
    void update(Mob *mob) override
    {
        m_stateContainer.update(mob);
        m_stateContainer.get()->update(mob);
    }
};

}
