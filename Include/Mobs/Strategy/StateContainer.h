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
        interfaceClass.enable();
        interfaceClass.disable();
        interfaceClass.update(mob);
    };

    template <typename T, typename U>
    requires is_required_interface<T>
    class StateContainer
    {
        using func_holder = std::unordered_map<size_t,
                std::vector< std::pair<std::function<bool(U*)>, std::shared_ptr<T>> > >;

    public:
        StateContainer() = delete;
        
        StateContainer(std::shared_ptr<T> currentState, const func_holder& functions): m_currentState(std::move(currentState)),
                                                                                            m_functions(functions){}
        StateContainer(StateContainer&& other) {
            m_currentState = std::move(other.m_currentState);
            m_functions = std::move(m_functions);
        }

        std::shared_ptr<T> get() const {
            return m_currentState;
        }
        void changeState(std::shared_ptr<T>&& newState) {
            m_currentState->disable();
            m_currentState = std::move(newState);
            m_currentState->enable();
        }
        void update(U* obj) {
            for (auto&& [function, state]: m_functions[typeid(*m_currentState).hash_code()])
            {
                if (function(obj))
                    changeState(std::move(state));
            }
        }

    private:
        func_holder m_functions;
        std::shared_ptr<T> m_currentState;
    };

} // mob
