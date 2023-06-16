#pragma once
#include "BaseStateStrategy.h"
#include "Mobs/Behaviour/IMobBehaviour.h"

namespace mob {
    /**
     * Normal state for mobs
     */
    class Normal: public BaseStateStrategy {
        std::shared_ptr<IMobBehaviour> startBehaviour;
        Normal() = delete;
    public:
        explicit Normal(std::shared_ptr<IMobBehaviour> startB) : startBehaviour(std::move(startB)) {}
        /**
         * Changes behaviour to start for mob
         * @param mob - pointer to mob
         */
        void enable(Mob* mob) override {
            mob->changeBehaviour(startBehaviour);
        }

        void disable(Mob*) override {}
            /**
             * Update state for mob
             */
        void update(Mob* mob) override {}
        /**
         * Get type id of normal state
         * @return type id of normal state
         */
        size_t getTypeId() override {
            return typeid(Normal).hash_code();
        }

    };
}