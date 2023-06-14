#include "Mobs/Factory/MobsConfig.h"
#include "ItemsSystem/Attacks.h"

using namespace mob;

AttackInfo::PossibleAttackDelegate dontHitObstacle = [](TileType tileType) { return tileType == TileType::GROUND; };
AttackInfo::PossibleAttackFromEntity hitOnlyPlayer = FunctionVisitorEntitiesBuilder<bool>().setPlayer([](Player*) { return true; }).build();

MobInfo MobsConfig::createNormalMobInfoWithLowPanic()
{
    const auto defaultAttackInfo = std::make_shared<AttackInfo>(dontHitObstacle, hitOnlyPlayer, nullptr, std::make_shared<AttackSearchFromDFS>());

    const std::shared_ptr<AttackHandler> attack = createAttack({{Attacks::simpleRange, defaultAttackInfo, 2}});

    transitions_type transitionFunction =
            {
                    { typeid(Normal).hash_code(),
                      {
                              { [](Mob *mob)
                                {
                                    std::shared_ptr<IStat> hpStat;
                                    std::shared_ptr<BoundsModificator> bounds;
                                    if (mob->getStats()->tryGet(StatType::HEALTH, hpStat) && hpStat->tryGet(bounds))
                                        return bounds->getValueFromPercent(hpStat->getValue()) < 30;
                                    else
                                        return false;
                                }
                                      , typeid(Panic).hash_code()
                              }
                      }
                    },
            };

    auto aggressiveBeh = createAggressiveBehaviour(attack);
    auto cowardlyBeh = createCowardlyBehaviour(attack);

    std::shared_ptr<BaseStateStrategy> normalState = std::make_shared<Normal>(aggressiveBeh);
    std::shared_ptr<BaseStateStrategy> panicState = std::make_shared<Panic>(cowardlyBeh);

    auto strategy = createStrategy(typeid(Normal).hash_code(), transitionFunction, normalState, panicState);
    return {10, 5, strategy, aggressiveBeh};
}



MobInfo MobsConfig::createNormalMobInfoWithHighPanic()
{
    const auto defaultAttackInfo = std::make_shared<AttackInfo>(dontHitObstacle, hitOnlyPlayer, nullptr, std::make_shared<AttackSearchFromDFS>());

    const std::shared_ptr<AttackHandler> attack = createAttack({{Attacks::simpleRange, defaultAttackInfo, 3}});

    transitions_type transitionFunction =
            {
                    { typeid(Normal).hash_code(),
                      {
                              { [](Mob *mob)
                                {
                                    std::shared_ptr<IStat> hpStat;
                                    std::shared_ptr<BoundsModificator> bounds;
                                    if (mob->getStats()->tryGet(StatType::HEALTH, hpStat) && hpStat->tryGet(bounds))
                                        return bounds->getValueFromPercent(hpStat->getValue()) < 60;
                                    else
                                        return false;
                                }
                                      , typeid(Panic).hash_code()
                              }
                      }
                    },
            };

    auto aggressiveBeh = createAggressiveBehaviour(attack);
    auto cowardlyBeh = createCowardlyBehaviour(attack);

    std::shared_ptr<BaseStateStrategy> normalState = std::make_shared<Normal>(aggressiveBeh);
    std::shared_ptr<BaseStateStrategy> panicState = std::make_shared<Panic>(cowardlyBeh);

    auto strategy = createStrategy(typeid(Normal).hash_code(), transitionFunction, normalState, panicState);
    return {7, 7, strategy, aggressiveBeh};
}



MobInfo MobsConfig::createEliteMobInfo()
{
    const auto defaultAttackInfo = std::make_shared<AttackInfo>(dontHitObstacle, hitOnlyPlayer, nullptr, std::make_shared<AttackSearchFromDFS>());
    const std::shared_ptr<AttackHandler> attack = createAttack({{Attacks::smallIRange, defaultAttackInfo, 5}});
    auto aggressiveBeh = createAggressiveBehaviour(attack);
    auto strategy = createStrategy();

    return {15, 6, strategy, aggressiveBeh};
}



MobInfo MobsConfig::createBossMobInfo()
{
    const auto defaultAttackInfo = std::make_shared<AttackInfo>(dontHitObstacle, hitOnlyPlayer, nullptr, std::make_shared<AttackSearchFromDFS>());
    const std::shared_ptr<AttackHandler> attack = createAttack({{Attacks::smallTRangeFlip, defaultAttackInfo, 7}});
    auto aggressiveBeh = createAggressiveBehaviour(attack);
    auto strategy = createStrategy();

    return {25, 7, strategy, aggressiveBeh};
}



MobInfo MobsConfig::createStrongPassiveMobInfo()
{
    const auto defaultAttackInfo = std::make_shared<AttackInfo>(dontHitObstacle, hitOnlyPlayer, nullptr, std::make_shared<AttackSearchFromDFS>());

    const std::shared_ptr<AttackHandler> attack = createAttack({{Attacks::closeLineRange, defaultAttackInfo, 2}});

    transitions_type transitionFunction =
            {
                    { typeid(Normal).hash_code(),
                      {
                              { [](Mob *mob)
                                {
                                    std::shared_ptr<IStat> hpStat;
                                    std::shared_ptr<BoundsModificator> bounds;
                                    if (mob->getStats()->tryGet(StatType::HEALTH, hpStat) && hpStat->tryGet(bounds))
                                        return bounds->getValueFromPercent(hpStat->getValue()) < 100;
                                    else
                                        return false;
                                }
                                      , typeid(Panic).hash_code()
                              }
                      }
                    },
            };

    auto aggressiveBeh = createAggressiveBehaviour(attack);
    auto passiveBeh = createPassiveBehaviour();

    std::shared_ptr<BaseStateStrategy> normalState = std::make_shared<Normal>(passiveBeh);
    std::shared_ptr<BaseStateStrategy> aggressiveState = std::make_shared<Panic>(aggressiveBeh);

    auto strategy = createStrategy(typeid(Normal).hash_code(), transitionFunction, normalState, aggressiveState);
    return {15, 4, strategy, passiveBeh};
}



MobInfo MobsConfig::createPassiveMobInfo()
{
    const auto defaultAttackInfo = std::make_shared<AttackInfo>(dontHitObstacle, hitOnlyPlayer, nullptr, std::make_shared<AttackSearchFromDFS>());

    const std::shared_ptr<AttackHandler> attack = createAttack({{Attacks::simpleRange, defaultAttackInfo, 1}});

    transitions_type transitionFunction =
            {
                    { typeid(Normal).hash_code(),
                      {
                              { [](Mob *mob)
                                {
                                    std::shared_ptr<IStat> hpStat;
                                    std::shared_ptr<BoundsModificator> bounds;
                                    if (mob->getStats()->tryGet(StatType::HEALTH, hpStat) && hpStat->tryGet(bounds))
                                        return bounds->getValueFromPercent(hpStat->getValue()) < 50;
                                    else
                                        return false;
                                }
                                      , typeid(Panic).hash_code()
                              }
                      }
                    },
            };

    auto aggressiveBeh = createAggressiveBehaviour(attack);
    auto passiveBeh = createPassiveBehaviour();

    std::shared_ptr<BaseStateStrategy> normalState = std::make_shared<Normal>(passiveBeh);
    std::shared_ptr<BaseStateStrategy> aggressiveState = std::make_shared<Panic>(aggressiveBeh);

    auto strategy = createStrategy(typeid(Normal).hash_code(), transitionFunction, normalState, aggressiveState);
    return {10, 5, strategy, passiveBeh};
}