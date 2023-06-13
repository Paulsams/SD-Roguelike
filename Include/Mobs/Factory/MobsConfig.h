#pragma once

#include "ItemsSystem/AttackSearch/AttackSearchFromDFS.h"
#include "Stats/Modificators/BoundsModificator.h"
#include "Mobs/Behaviour/AggressiveBehaviour.h"
#include "Mobs/Strategy/StrategyWithStates.h"
#include "Mobs/Behaviour/CowardlyBehaviour.h"
#include "Mobs/Behaviour/PassiveBehaviour.h"
#include "Mobs/Strategy/NothingStrategy.h"
#include "Mobs/Strategy/Normal.h"
#include "Mobs/Factory/MobInfo.h"
#include "Mobs/Strategy/Panic.h"
#include <concepts>
#include <unordered_map>
#include <functional>
#include "Mobs/Mob.h"

namespace mob {

    template<typename T>
    concept has_hash_code = requires(T &&args) {
        args->getTypeId();
    };

    class MobsConfig {
        using transitions_type = std::unordered_map<size_t, std::vector<std::pair<std::function<bool(Mob *)>, size_t>>>;
        template<typename T>
        using shared_type = std::remove_reference_t<decltype(*std::declval<std::decay_t<T>>())>;

        std::shared_ptr<IMobBehaviour> createCowardlyBehaviour(std::shared_ptr<AttackHandler> attack) {
            return std::make_shared<CowardlyBehaviour>(attack);
        };

        std::shared_ptr<IMobBehaviour> createPassiveBehaviour(std::shared_ptr<AttackHandler> attack) {
            return std::make_shared<PassiveBehaviour>();
        };

        std::shared_ptr<IMobBehaviour> createAggressiveBehaviour(std::shared_ptr<AttackHandler> attack) {
            return std::make_shared<AggressiveBehaviour>(std::move(attack));
        }

        std::shared_ptr<BaseStateStrategy> createNormalState(std::shared_ptr<IMobBehaviour> behaviour) {
            return std::make_shared<Normal>(std::move(behaviour));
        }

        std::shared_ptr<BaseStateStrategy> createPanicState(std::shared_ptr<IMobBehaviour> behaviour) {
            return std::make_shared<Panic>(std::move(behaviour));
        }

        template<typename... T>
        requires ((std::is_same_v<shared_type<T>, BaseStateStrategy> && ...)
                  && sizeof...(T) > 0 && (has_hash_code<T> && ...))
        std::shared_ptr<IMobStrategy> createStrategy(size_t startState, const transitions_type &transitionFunction, T... states) {

            std::unordered_map<size_t, std::shared_ptr<std::common_type_t<shared_type<T>...>>> statesContainer;
            for (auto &state: {states...}) {
                statesContainer[state->getTypeId()] = state;
            }
            StateMachine<BaseStateStrategy, Mob> stateContainer(statesContainer.at(startState), statesContainer,
                                                                transitionFunction);
            return std::make_shared<StrategyWithStates>(std::move(stateContainer));
        };

        template<typename... T>
        requires (sizeof ...(T) == 0)
        std::shared_ptr<IMobStrategy> createStrategy(T... states) {
            return std::make_shared<NothingStrategy>();
        };


        MobInfo createMobInfoOrc() {
            AttackInfo::PossibleAttackDelegate dontHitObstacle =
                    [](TileType tileType) { return tileType == TileType::GROUND; };

            AttackInfo::PossibleAttackFromEntity hitOnlyPlayer = FunctionVisitorEntitiesBuilder<bool>()
                    .setPlayer([](Player *) { return true; })
                    .build();

            const auto defaultAttackInfo = std::make_shared<AttackInfo>(dontHitObstacle, hitOnlyPlayer,
                                                                        nullptr,
                                                                        std::make_shared<AttackSearchFromDFS>());

            const std::vector<Vec2Int> oneDistanceRange = {{1, 0}};
            const std::shared_ptr<AttackHandler> orcWarriorAttack =
                    createAttack({{oneDistanceRange, defaultAttackInfo, 3}});


            auto aggressiveBeh = createAggressiveBehaviour(orcWarriorAttack);
            auto cowardlyBeh = createCowardlyBehaviour(orcWarriorAttack);

            transitions_type transitionFunction = {
                    {typeid(Normal).hash_code(),
                     {
                             {[](Mob *mob) {
                                 std::shared_ptr<IStat> hpStat;
                                 std::shared_ptr<BoundsModificator> bounds;
                                 if (mob->getStats()->tryGet(StatType::Health, hpStat) && hpStat->tryGet(bounds))
                                     return bounds->getValueFromPercent(hpStat->getValue()) < 40;
                                 else
                                     return false;
                             }, typeid(Panic).hash_code()}
                     }
                    },
            };

            std::shared_ptr<BaseStateStrategy> normalState = std::make_shared<Normal>(aggressiveBeh);
            std::shared_ptr<BaseStateStrategy> panicState = std::make_shared<Panic>(cowardlyBeh);

            auto strategy = createStrategy(typeid(Normal).hash_code(), transitionFunction, normalState, panicState);
            return {15, 5, strategy, aggressiveBeh};
        }

    public:
        MobsConfig() {
            normals.insert({
                {176, createMobInfoOrc()}
            });
        }

        std::shared_ptr<AttackHandler> createAttack(const std::vector<std::tuple<const std::vector<Vec2Int> &,
                std::shared_ptr<AttackInfo>, float>> &ranges) {
            AttackHandlerBuilder builder;
            for (auto [range, attackInfo, damage]: ranges) {
                builder.addAttackData(range, std::make_shared<AttackWithDamage>(attackInfo,
                                                                                std::make_shared<Damage>(damage)));
            }
            return builder.build();
        }

        std::unordered_map<int, MobInfo> normals;
        std::unordered_map<int, MobInfo> elites;
        std::unordered_map<int, MobInfo> bosses;
        std::unordered_map<int, MobInfo> passives;
    };
}
