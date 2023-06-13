#pragma once
#include <memory>
#include <unordered_map>
#include <vector>
#include <functional>

#include "Mobs/Mob.h"

namespace mob
{
    template<typename T>
    concept is_required_interface = requires (T&& interfaceClass, Mob* mob)
    {
        interfaceClass.enable(mob);
        interfaceClass.disable(mob);
        interfaceClass.update(mob);
        interfaceClass.getTypeId();
    };

    template <typename T, typename U>
    requires is_required_interface<T>
    class StateMachine
    {
        using func_holder = std::unordered_map<size_t,
                std::vector< std::pair<std::function<bool(U*)>, size_t> > >;
        using state_holder = std::unordered_map<size_t, std::shared_ptr<T>>;
    public:
        StateMachine() = delete;
        
        StateMachine(std::shared_ptr<T> currentState, const state_holder& states, const func_holder& functions):
                                                                                            m_currentState(std::move(currentState)),
                                                                                            m_functions(functions),
                                                                                            m_states(states)
                                                                                            {}
        StateMachine(StateMachine&& other) {
            m_currentState = std::move(other.m_currentState);
            m_states = std::move(other.m_states);
            m_functions = std::move(other.m_functions);
        }

        std::shared_ptr<T> get() const {
            return m_currentState;
        }
        void changeState(size_t newState_id, U* mob) {
            m_currentState->disable(mob);
            m_currentState = m_states[newState_id];
            m_currentState->enable(mob);
        }
        void update(U* obj) {
            for (auto& [function, state_id]: m_functions[m_currentState->getTypeId()]) {
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
