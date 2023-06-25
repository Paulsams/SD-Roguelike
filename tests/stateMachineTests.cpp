#include "Mobs/Strategy/StateMachine.h"
#include "Stats/Modificators/BoundsModificator.h"

#include <gtest/gtest.h>

#include <unordered_map>
#include <memory>

struct test_struct
{
    int health = 100;
};

struct test_state
{
    virtual void disable(test_struct*) {}
    virtual void enable(test_struct*) {}
    virtual void update(test_struct*) {}
    virtual std::shared_ptr<test_state> clone() const = 0;
    virtual size_t getTypeId() = 0;
};

struct normal : test_state
{
    virtual std::shared_ptr<test_state> clone() const override
    {
        return std::make_shared<normal>();
    }

    virtual size_t getTypeId()
    {
        return typeid(normal).hash_code();
    }
};

struct panic: test_state
{
    virtual std::shared_ptr<test_state> clone() const override
    {
        return std::make_shared<panic>();
    }

    virtual size_t getTypeId()
    {
        return typeid(panic).hash_code();
    }
};

using transitions_type = std::unordered_map<size_t, std::vector<std::pair<std::function<bool(test_struct*)>, size_t>>>;


TEST(StateMachineTest, stateMachineTest) {

    transitions_type transitionFunction = {
            {typeid(normal).hash_code(),
             {
                     {[](test_struct *mob) {
                         return dynamic_cast<test_struct*>(mob)->health < 50;

                     }, typeid(panic).hash_code()}
             }
            },
    };
    std::shared_ptr<test_state> normal_state = std::make_shared<normal>();
    std::shared_ptr<test_state> panic_state = std::make_shared<panic>();
    std::unordered_map<size_t, std::shared_ptr<test_state>> statesContainer = { {typeid(normal).hash_code(), normal_state},
                                                                                            {typeid(panic).hash_code(), panic_state}};
    size_t startState = typeid(normal).hash_code();
    mob::StateMachine<test_state, test_struct> stateMachine(statesContainer.at(startState), statesContainer,
                                                        transitionFunction);
    test_struct testObj;
    EXPECT_EQ(testObj.health, 100);
    EXPECT_EQ(stateMachine.get()->getTypeId(), typeid(normal).hash_code());
    testObj.health = 70;
    EXPECT_EQ(testObj.health, 70);
    EXPECT_EQ(stateMachine.get()->getTypeId(), typeid(normal).hash_code());
    testObj.health = 40;
    stateMachine.update(&testObj);
    EXPECT_EQ(testObj.health, 40);
    EXPECT_EQ(stateMachine.get()->getTypeId(), typeid(panic).hash_code());
}
