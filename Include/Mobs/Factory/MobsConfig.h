#pragma once

#include "ItemsSystem/AttackSearch/AttackSearchFromStraightLines.h"
#include "Stats/Modificators/BoundsModificator.h"
#include "Mobs/Behaviour/AggressiveBehaviour.h"
#include "Mobs/Strategy/StrategyWithStates.h"
#include "Mobs/Behaviour/CowardlyBehaviour.h"
#include "Mobs/Behaviour/PassiveBehaviour.h"
#include "Mobs/Strategy/NothingStrategy.h"
#include "Mobs/Strategy/Normal.h"
#include "Mobs/Factory/MobInfo.h"
#include "Mobs/Strategy/Panic.h"
#include "Mobs/Mob.h"

#include <concepts>
#include <functional>
#include <unordered_map>


namespace mob
{

template <typename T>
concept has_hash_code = requires(T &&args) { args->getTypeId(); };

class MobsConfig
{
private:

    using transitions_type = std::unordered_map<size_t, std::vector<std::pair<std::function<bool(Mob*)>, size_t>>>;

    template<typename T>
    using shared_type = std::remove_reference_t<decltype(*std::declval<std::decay_t<T>>())>;

    std::shared_ptr<IMobBehaviour> createCowardlyBehaviour(std::shared_ptr<AttackHandler> attack)
    {
        return std::make_shared<CowardlyBehaviour>(std::move(attack));
    };

    std::shared_ptr<IMobBehaviour> createPassiveBehaviour()
    {
        return std::make_shared<PassiveBehaviour>();
    };

    std::shared_ptr<IMobBehaviour> createAggressiveBehaviour(std::shared_ptr<AttackHandler> attack)
    {
        return std::make_shared<AggressiveBehaviour>(std::move(attack));
    }

    std::shared_ptr<BaseStateStrategy> createNormalState(std::shared_ptr<IMobBehaviour> behaviour)
    {
        return std::make_shared<Normal>(std::move(behaviour));
    }

    std::shared_ptr<BaseStateStrategy> createPanicState(std::shared_ptr<IMobBehaviour> behaviour)
    {
        return std::make_shared<Panic>(std::move(behaviour));
    }

    template<typename... T>
    requires ((std::is_same_v<shared_type<T>, BaseStateStrategy> && ...) && sizeof...(T) > 0 && (has_hash_code<T> && ...))
    std::shared_ptr<IMobStrategy> createStrategy(size_t startState, const transitions_type& transitionFunction, T&&... states)
    {
        std::unordered_map<size_t, std::shared_ptr<std::common_type_t<shared_type<T>...>>> statesContainer;
        for (const auto& state: {states...})
        {
            statesContainer[state->getTypeId()] = state;
        }
        StateMachine<BaseStateStrategy, Mob> stateContainer(statesContainer.at(startState), statesContainer, transitionFunction);
        return std::make_shared<StrategyWithStates>(std::move(stateContainer));
    };

    template<typename... T>
    requires (sizeof ...(T) == 0)
    std::shared_ptr<IMobStrategy> createStrategy(T&&... states)
    {
        return std::make_shared<NothingStrategy>();
    };


    MobInfo createNormalMobInfoWithLowPanic();
    MobInfo createNormalMobInfoWithHighPanic();
    MobInfo createEliteMobInfo();
    MobInfo createBossMobInfo();
    MobInfo createStrongPassiveMobInfo();
    MobInfo createPassiveMobInfo();

public:
    MobsConfig()
    {
        normals.insert({
            {174, createNormalMobInfoWithHighPanic()},
            {175, createNormalMobInfoWithHighPanic()},
            {176, createNormalMobInfoWithLowPanic()},
            {177, createNormalMobInfoWithHighPanic()},
            {178, createNormalMobInfoWithHighPanic()},
            {179, createNormalMobInfoWithLowPanic()},
            {180, createNormalMobInfoWithLowPanic()},
            {181, createNormalMobInfoWithHighPanic()},
        });

        elites.insert({
            {376, createEliteMobInfo()},
            {377, createEliteMobInfo()},
            {378, createEliteMobInfo()},
            {379, createEliteMobInfo()},
            {380, createEliteMobInfo()},
            {381, createEliteMobInfo()},
            {382, createEliteMobInfo()},
            {383, createEliteMobInfo()},
            {384, createEliteMobInfo()},
        });

        bosses.insert({
            {129, createBossMobInfo()},
            {131, createBossMobInfo()},
            {320, createBossMobInfo()},
        });

        passives.insert({
            {228, createPassiveMobInfo()},
            {242, createPassiveMobInfo()},
            {293, createStrongPassiveMobInfo()},
            {295, createPassiveMobInfo()},
            {307, createStrongPassiveMobInfo()},
        });
    }

    std::shared_ptr<AttackHandler> createAttack(const std::vector<std::tuple<const std::vector<Vec2Int>, std::shared_ptr<AttackInfo>, float>>& ranges)
    {
        AttackHandlerBuilder builder;
        for (const auto& [range, attackInfo, damage]: ranges)
        {
            builder.addAttackData(range, std::make_shared<AttackWithDamage>(attackInfo, std::make_shared<Damage>(damage)));
        }
        return builder.build();
    }

    std::unordered_map<int, MobInfo> normals;
    std::unordered_map<int, MobInfo> elites;
    std::unordered_map<int, MobInfo> bosses;
    std::unordered_map<int, MobInfo> passives;
};

}
