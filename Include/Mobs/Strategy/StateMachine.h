#pragma once

#include "Mobs/Mob.h"

#include <memory>
#include <unordered_map>
#include <vector>
#include <functional>


namespace mob
{

/**
 * Check if T satisfies mob state machine state class
 * @tparam T interface
 * @tparam U mob class
 */
template<typename T, typename U>
concept is_required_interface = requires (T&& interfaceClass, U* mob)
{
    interfaceClass.enable(mob);
    interfaceClass.disable(mob);
    interfaceClass.update(mob);
    interfaceClass.getTypeId();
    { interfaceClass.clone() } -> std::same_as<std::shared_ptr<T>>;
};


/**
 * Mob state machine class
 */
template <typename T, typename U>
requires is_required_interface<T, U>
class StateMachine
{
private:
    using func_holder = std::unordered_map<size_t, std::vector<std::pair<std::function<bool(U*)>, size_t>>>;
    using state_holder = std::unordered_map<size_t, std::shared_ptr<T>>;
public:
    StateMachine() = delete;

    StateMachine(std::shared_ptr<T> currentState, const state_holder& states, const func_holder& functions)
        : m_currentState(std::move(currentState))
        , m_functions(functions)
        , m_states(states)
    {}

    StateMachine(StateMachine&& other) noexcept = default;

    StateMachine(const StateMachine& other)
        : m_functions(other.m_functions)
        , m_currentState(other.m_currentState->clone())
    {
        for (const auto& [key, state] : other.m_states)
        {
            m_states[key] = other.m_states.at(key)->clone();
        }
    }

    /**
     * @return current state
     */
    std::shared_ptr<T> get() const { return m_currentState; }

    /**
     * Change current state to new state and perform disable of current state and enable of new state
     * @param newState_id new state id
     * @param mob mob
     */
    void changeState(size_t newState_id, U* mob)
    {
        m_currentState->disable(mob);
        m_currentState = m_states[newState_id];
        m_currentState->enable(mob);
    }

    /**
     * Update obj and change current state
     */
    void update(U* obj)
    {
        for (auto& [function, state_id]: m_functions[m_currentState->getTypeId()])
        {
            if (function(obj))
                changeState(state_id, obj);
        }
    }

private:
    func_holder m_functions;
    state_holder m_states;
    std::shared_ptr<T> m_currentState;
};

} // mob
