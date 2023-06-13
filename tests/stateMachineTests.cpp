#include "../Include/Mobs/Strategy/StateMachine.h"
#include "../Include/Mobs/Strategy/Normal.h"
#include "../Include/Mobs/Strategy/Panic.h"
#include "Stats/Modificators/BoundsModificator.h"

#include "Mobs/Factory/MobsConfig.h"
#include <gtest/gtest.h>

struct test_struct {
    int health = 100;
};
using transitions_type = std::unordered_map<size_t, std::vector<std::pair<std::function<bool(test_struct*)>, size_t>>>;


TEST(StateMachineTest, stateMachineTest) {

//    std::shared_ptr<mob::BaseStateStrategy> normalState = std::make_shared<mob::Normal>(nullptr);
//    std::shared_ptr<mob::BaseStateStrategy> panicState = std::make_shared<mob::Panic>(nullptr);
//
//    transitions_type transitionFunction = {
//            {typeid(mob::Normal).hash_code(),
//             {
//                     {[](test_struct *mob) {
//                         return mob->health < 50;
//
//                     }, typeid(mob::Panic).hash_code()}
//             }
//            },
//    };
//
////    auto strategy = createStrategy(typeid(mob::Normal).hash_code(), transitionFunction, normalState, panicState);
//    StateMachine<BaseStateStrategy, Mob> stateContainer(statesContainer.at(startState), statesContainer,
//                                                        transitionFunction);

}
