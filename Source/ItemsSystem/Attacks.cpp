#include "ItemsSystem/Attacks.h"
#include "ItemsSystem/AttackSearch/AttackSearchFromDFS.h"

inline const WeaponInfo Attacks::defaultWeapon = {3010, 1};
inline const WeaponInfo Attacks::stick_1 = {2943, 1};
inline const WeaponInfo Attacks::axe_1 = {1766, 1};

Attacks::Attacks()
{
    if (m_createWeapons.size() != 0)
        return;

    initWeaponsAndAttacks();
}

void Attacks::initWeaponsAndAttacks()
{
    AttackInfo::PossibleAttackDelegate dontHitObstacle =
        [](TileType tileType) { return tileType == TileType::GROUND || tileType == TileType::DECORATION; };
    
    AttackInfo::PossibleAttackFromEntity hitMobsAndDecorations = FunctionVisitorEntitiesBuilder<bool>()
        .setMob([](mob::Mob*) { return true; })
        .setDecoration([](Decoration*) { return true; })
        .build();

    const auto defaultAttackInfo = std::make_shared<AttackInfo>(dontHitObstacle, hitMobsAndDecorations,
        nullptr, std::make_shared<AttackSearchFromDFS>());

    const std::vector<Vec2Int> oneDistanceRange = {{1, 0}};
    addNewWeapon(defaultWeapon, {{oneDistanceRange, defaultAttackInfo, 1}});

    const std::vector<Vec2Int> rightOneDistanceRange = {{1, 0}, {1, -1}};
    addNewWeapon(stick_1, {{rightOneDistanceRange, defaultAttackInfo, 1}});

    const std::vector<Vec2Int> axeRange = {{3, 1}, {3, 0}, {3, -1}};
    addNewWeapon(axe_1, {{axeRange, defaultAttackInfo, 3}});
}

void Attacks::addNewWeapon(WeaponInfo info,
    std::vector<std::tuple<const std::vector<Vec2Int>, std::shared_ptr<AttackInfo>, float>> ranges)
{
    m_createWeapons[info.gid].insert({info.tier,
        [info, ranges](World* world) {
            AttackHandlerBuilder attackBuilder;
            std::vector<std::shared_ptr<Damage>> damages;
            damages.reserve(ranges.size());
            for (auto [range, attackInfo, damage] : ranges)
            {
                damages.push_back(std::make_shared<Damage>(damage));
                attackBuilder.addAttackData(range, std::make_shared<AttackWithDamage>(attackInfo, damages.back()));
            }
            const auto attack = attackBuilder.build();
            
            return Weapon::create(world, World::getRectFromGid(info.gid), WEAPON, attack, damages);
        }});
}
