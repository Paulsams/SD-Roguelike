#include "ItemsSystem/Attacks.h"
#include "ItemsSystem/AttackSearch/AttackSearchFromDFS.h"

inline const std::string Attacks::defaultWeapon = "Default";

inline const Attacks Attacks::instance = Attacks();

Attacks::Attacks()
{
    initWeaponsAndAttacks();
}

std::unordered_map<int, std::vector<std::string>> Attacks::getTiers()
{
    std::unordered_map<int, std::vector<std::string>> result;
    for (auto [name, createFunctions] : instance.m_createWeapons)
    {
        std::vector<int> tiers(createFunctions.size());
        for (auto [tier, func] : createFunctions)
            result[tier].push_back(name);
    }
    return result;
}

void Attacks::initWeaponsAndAttacks()
{
    static const std::string stick = "Stick";
    static const std::string berdysh = "Berdysh";
    
    AttackInfo::PossibleAttackDelegate dontHitObstacle =
        [](TileType tileType) { return tileType == TileType::GROUND || tileType == TileType::DECORATION; };
    
    AttackInfo::PossibleAttackFromEntity hitMobsAndDecorations = FunctionVisitorEntitiesBuilder<bool>()
        .setMob([](mob::Mob*) { return true; })
        .setDecoration([](Decoration*) { return true; })
        .build();

    const auto defaultAttackInfo = std::make_shared<AttackInfo>(dontHitObstacle, hitMobsAndDecorations,
        nullptr, std::make_shared<AttackSearchFromDFS>());

    const std::vector<Vec2Int> oneDistanceRange = {{1, 0}};
    addNewWeapon(defaultWeapon, 1, 3010, {{oneDistanceRange, defaultAttackInfo, 1}});

    const std::vector<Vec2Int> rightOneDistanceRange = {{1, 0}, {1, -1}};
    addNewWeapon(stick, 1, 2943, {{rightOneDistanceRange, defaultAttackInfo, 1}});

    const std::vector<Vec2Int> axeRange = {{3, 1}, {3, 0}, {3, -1}};
    addNewWeapon(berdysh, 1, 1766, {{axeRange, defaultAttackInfo, 3}});
}

void Attacks::addNewWeapon(const std::string& name, int tier, int gid,
    std::vector<std::tuple<const std::vector<Vec2Int>, std::shared_ptr<AttackInfo>, float>> ranges)
{
    m_createWeapons[name].insert({tier,
        [ranges, gid](World* world) {
            AttackHandlerBuilder attackBuilder;
            std::vector<std::shared_ptr<Damage>> damages;
            damages.reserve(ranges.size());
            for (auto [range, attackInfo, damage] : ranges)
            {
                damages.push_back(std::make_shared<Damage>(damage));
                attackBuilder.addAttackData(range, std::make_shared<AttackWithDamage>(attackInfo, damages.back()));
            }
            const auto attack = attackBuilder.build();
            
            return Weapon::create(world, world->getRectFromGid(gid), WEAPON, attack, damages);
        }});
}
