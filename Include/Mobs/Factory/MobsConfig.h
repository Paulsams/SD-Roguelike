#pragma once

#include "Mobs/Behaviour/AggressiveBehaviour.h"
#include "Mobs/Strategy/StrategyWithStates.h"
#include "Mobs/Behaviour/CowardlyBehaviour.h"
#include "Mobs/Behaviour/PassiveBehaviour.h"
#include "Mobs/Strategy/Normal.h"
#include "Mobs/Factory/MobInfo.h"
#include <unordered_map>
#include <functional>

#include "ItemsSystem/AttackSearch/AttackSearchFromDFS.h"
#include "Mobs/Mob.h"
#include "Mobs/Strategy/NothingStrategy.h"

namespace mob {

    class MobsConfig {

        std::unordered_map<size_t, std::vector<std::pair<std::function<bool(Mob*)>, std::shared_ptr<BaseStateStrategy>>>> transitionFunction = {
                { typeid(Normal).hash_code(),
                  {
                    { [](Mob *mob) {
                        std::shared_ptr<IStat> hpStat;
                        if (mob->getStats()->tryGet(StatType::HEALTH, hpStat))
                            return hpStat->getValue() < 50;
                        else
                            return false;
                    }, std::make_shared<Normal>() }
                  }
                },
        };

        std::function<std::shared_ptr<IMobStrategy>()> createStrategyWithNormal = [&]() {
            StateMachine<BaseStateStrategy, Mob> stateContainer(std::make_shared<Normal>(), transitionFunction);
            return std::make_shared<StrategyWithStates>(std::move(stateContainer));
        };

        std::function<std::shared_ptr<IMobStrategy>()> createNothingStrategy = []()
        {
            return std::make_shared<NothingStrategy>();
        };

        std::function<std::shared_ptr<IMobBehaviour>()> createCowardlyBehaviour = [](){
            std::shared_ptr<AttackHandler> attack = nullptr;
            return std::make_shared<CowardlyBehaviour>(attack);
        };

        std::function<std::shared_ptr<IMobBehaviour>()> createPassiveBehaviour = [](){
            return std::make_shared<PassiveBehaviour>();
        };

    public:
        MobsConfig()
        {
            AttackInfo::PossibleAttackDelegate dontHitObstacle =
                [](TileType tileType) { return tileType == TileType::GROUND; };
    
            AttackInfo::PossibleAttackFromEntity hitOnlyPlayer = FunctionVisitorEntitiesBuilder<bool>()
                .setPlayer([](Player*) { return true; })
                .build();

            const auto defaultAttackInfo = std::make_shared<AttackInfo>(dontHitObstacle, hitOnlyPlayer,
                nullptr, std::make_shared<AttackSearchFromDFS>());

            const std::vector<Vec2Int> oneDistanceRange = {{1, 0}};
            const std::shared_ptr<AttackHandler> orcWarriorAttack =
                createAttack({{oneDistanceRange, defaultAttackInfo, 3}});

            MobInfo orcWarrior(15, 5,
                createNothingStrategy, createAggressiveBehaviour(orcWarriorAttack));
            
            normals.insert({
                {176, orcWarrior}
            });
        }

        std::shared_ptr<AttackHandler> createAttack(const std::vector<std::tuple<const std::vector<Vec2Int>&,
            std::shared_ptr<AttackInfo>, float>>& ranges)
        {
            AttackHandlerBuilder builder;
            for (auto [range, attackInfo, damage] : ranges)
            {
                builder.addAttackData(range, std::make_shared<AttackWithDamage>(attackInfo,
                    std::make_shared<Damage>(damage)));
            }
            return builder.build();
        }

        std::function<std::shared_ptr<IMobBehaviour>()> createAggressiveBehaviour(std::shared_ptr<AttackHandler> attack)
        {
            return [attack]() { return std::make_shared<AggressiveBehaviour>(attack); };
        }
        
        std::unordered_map<int, MobInfo> normals;
        std::unordered_map<int, MobInfo> elites;
        std::unordered_map<int, MobInfo> bosses;
        std::unordered_map<int, MobInfo> passives;
    };
}
