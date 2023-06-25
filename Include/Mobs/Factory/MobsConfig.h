#pragma once

#include "ItemsSystem/AttackSearch/AttackSearchFromStraightLines.h"
#include "Stats/Modificators/BoundsModificator.h"
#include "Mobs/Behaviour/AggressiveBehaviour.h"
#include "Mobs/Strategy/StrategyWithStates.h"
#include "Mobs/Behaviour/CowardlyBehaviour.h"
#include "Mobs/Behaviour/PassiveBehaviour.h"
#include "Mobs/Behaviour/ReplicativeBehaviour.h"
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

/**
 * Check if T is hashable
 */
template <typename T>
concept has_hash_code = requires(T&& args) { args->getTypeId(); };


/**
 * Configuration of all mobs in the game
 */
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

    std::shared_ptr<IMobBehaviour> createReplicativeBehaviour(std::shared_ptr<AttackHandler> attack)
    {
        return std::make_shared<ReplicativeBehaviour>(std::move(attack));
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


    MobInfo createNormalMobInfoWithLowPanic(GID gid);
    MobInfo createNormalMobInfoWithHighPanic(GID gid);
    MobInfo createEliteMobInfo(GID gid);
    MobInfo createBossMobInfo(GID gid);
    MobInfo createStrongPassiveMobInfo(GID gid);
    MobInfo createPassiveMobInfo(GID gid);

public:
    MobsConfig()
    {
        normals.insert({
            {174, createNormalMobInfoWithHighPanic(174)},
            {175, createNormalMobInfoWithHighPanic(175)},
            {176, createNormalMobInfoWithLowPanic(176)},
            {177, createNormalMobInfoWithHighPanic(177)},
            {178, createNormalMobInfoWithHighPanic(178)},
            {179, createNormalMobInfoWithLowPanic(179)},
            {180, createNormalMobInfoWithLowPanic(180)},
            {181, createNormalMobInfoWithHighPanic(181)},
        });

        elites.insert({
            {376, createEliteMobInfo(376)},
            {377, createEliteMobInfo(377)},
            {378, createEliteMobInfo(378)},
            {379, createEliteMobInfo(379)},
            {380, createEliteMobInfo(380)},
            {381, createEliteMobInfo(381)},
            {382, createEliteMobInfo(382)},
            {383, createEliteMobInfo(383)},
            {384, createEliteMobInfo(384)},
        });

        bosses.insert({
            {129, createBossMobInfo(129)},
            {131, createBossMobInfo(131)},
            {320, createBossMobInfo(320)},
        });

        passives.insert({
            {228, createPassiveMobInfo(228)},
            {242, createPassiveMobInfo(242)},
            {293, createStrongPassiveMobInfo(293)},
            {295, createPassiveMobInfo(295)},
            {307, createStrongPassiveMobInfo(307)},
        });
    }

    /**
     * Create attack with given ranges
     * @param ranges ranges
     * @return AttackHandler
     */
    std::shared_ptr<AttackHandler> createAttack(const std::vector<std::tuple<const std::vector<Vec2Int>, std::shared_ptr<AttackInfo>, float>>& ranges)
    {
        AttackHandlerBuilder builder;
        for (const auto& [range, attackInfo, damage]: ranges)
        {
            builder.addAttackData(range, std::make_shared<AttackWithDamage>(attackInfo, std::make_shared<Damage>(damage)));
        }
        return builder.build();
    }

    std::unordered_map<int, MobInfo> normals; /// normal mobs: GID to info
    std::unordered_map<int, MobInfo> elites; /// elite mobs: GID to info
    std::unordered_map<int, MobInfo> bosses; /// boss mobs: GID to info
    std::unordered_map<int, MobInfo> passives; /// passive mobs: GID to info
};

}
