#pragma once

#include "Mobs/Behaviour/AggressiveBehaviour.h"
#include "Mobs/Strategy/StrategyWithStates.h"
#include "Mobs/Behaviour/CowardlyBehaviour.h"
#include "Mobs/Strategy/Normal.h"
#include "Mobs/Factory/MobInfo.h"
#include <unordered_map>
#include <functional>
#include "Mobs/Mob.h"
#include "MobInfo.h"
#include <cstdint>

namespace mob {

    class MobsConfig {

        std::unordered_map<size_t, std::vector<std::pair<std::function<bool(Mob*)>, std::shared_ptr<BaseStateStrategy>>>> transitionFunction = {
                { typeid(Normal).hash_code(),
                  {
                    { [](Mob *mob) { std::shared_ptr<IStat> hpStat;
                        if (mob->getStats()->tryGet(StatType::Health, hpStat))
                            return hpStat->getValue() < 50;
                        else
                            return false;}, std::make_shared<Normal>() }
                  }
                },
        };

        std::function<std::shared_ptr<IMobStrategy>()> createStrategyWithNormal = [&]() {
            StateContainer<BaseStateStrategy, Mob> stateContainer(std::make_shared<Normal>(), transitionFunction);
            return std::make_shared<StrategyWithStates>(std::move(stateContainer));
        };

        std::function<std::shared_ptr<IMobBehaviour>()> createAggressiveBehaviour = [&] () {
            std::shared_ptr<AttackHandler> attack = nullptr;
            return std::make_shared<AggressiveBehaviour>(attack);
        };
    public:
        std::unordered_map<int , MobInfo> normals = {
                {10 , MobInfo(100, 10, createStrategyWithNormal, createAggressiveBehaviour)},
        };

        std::unordered_map<int, MobInfo> elites = {
                {10 , MobInfo(100, 5, createStrategyWithNormal, createAggressiveBehaviour)},
        };
        std::unordered_map<int, MobInfo> bosses = {
                {10, MobInfo(100, 2, createStrategyWithNormal, createAggressiveBehaviour)},
        };

    };
}