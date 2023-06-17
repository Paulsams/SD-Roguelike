#include "ItemsSystem/Attacks.h"
#include "ItemsSystem/AttackSearch/AttackSearchFromStraightLines.h"

inline const Attacks Attacks::instance = Attacks();

Attacks::Attacks()
{
    initWeaponsAndAttacks();
}

std::vector<std::pair<std::string, int>> Attacks::getTiers()
{
    std::vector<std::pair<std::string, int>> result;
    for (const auto& [name, createFunctions] : instance.m_createWeapons)
    {
        std::vector<int> tiers(createFunctions.size());
        for (const auto& [tier, func] : createFunctions)
            result.emplace_back(name, tier);
    }
    return result;
}

void Attacks::initWeaponsAndAttacks()
{
    static const std::string stick = "Stick";
    static const std::string haberdasher = "Haberdasher";
    static const std::string sekira1 = "Sekira";
    static const std::string sekira2 = "2-side Sekira";
    static const std::string excalibur = "Excalibur";
    static const std::string whip = "Whip";
    static const std::string bat = "Bat";
    static const std::string dirk = "Dirk";
    static const std::string kingsword = "Kingsword";
    static const std::string morgenstern1 = "Morgenstern";
    static const std::string morgenstern2 = "Chained Morgenstern";
    static const std::string morgenstern3 = "Big Chained Morgenstern";
    static const std::string bigSekira1 = "Big Sekira";
    static const std::string bigSekira2 = "Big Sekira with pattern";
    static const std::string saber = "Saber";
    static const std::string lance = "Lance";
    static const std::string sword = "Sword";
    static const std::string pike = "Pike";
    static const std::string hammer = "Hammer";
    static const std::string axe = "Axe";
    static const std::string katana = "Katana";
    static const std::string knife = "Knife";
    static const std::string gigaSword = "Gigasword";
    static const std::string trident = "Trident";
    static const std::string scythe = "Scythe";


    AttackInfo::PossibleAttackDelegate dontHitObstacle =
        [](TileType tileType) { return tileType == TileType::GROUND || tileType == TileType::DECORATION; };
    
    AttackInfo::PossibleAttackFromEntity hitMobsAndDecorations = FunctionVisitorEntitiesBuilder<bool>()
        .setMob([](mob::Mob*) { return true; })
        .setDecoration([](Decoration*) { return true; })
        .build();

    const auto defaultAttackInfo = std::make_shared<AttackInfo>(dontHitObstacle, hitMobsAndDecorations, nullptr, std::make_shared<AttackSearchFromStraightLines>());

    m_createDefault = createCreatedFunc(3010, {{simpleRange, defaultAttackInfo, 1}});

    addNewWeapon(stick, 1, 2943, {{simpleRange, defaultAttackInfo, 2}});

    addNewWeapon(haberdasher, 1, 1766, {{ARange, defaultAttackInfo, 3}});
    addNewWeapon(haberdasher, 2, 1767, {{ARange, defaultAttackInfo, 5}});

    addNewWeapon(sekira2, 1, 1768, {{close3LineRange, defaultAttackInfo, 2}, {smallTRangeFlip, defaultAttackInfo, 2}});
    addNewWeapon(sekira2, 2, 1769, {{close3LineRange, defaultAttackInfo, 3}, {smallTRangeFlip, defaultAttackInfo, 3}});
    addNewWeapon(sekira2, 3, 1770, {{close3LineRange, defaultAttackInfo, 4}, {smallTRangeFlip, defaultAttackInfo, 4}});

    addNewWeapon(excalibur, 4, 1771, {{bigTRangeFlip, defaultAttackInfo, 7}});

    addNewWeapon(sekira1, 1, 1772, {{close2LineRange, defaultAttackInfo, 3}, {smallTRangeFlip, defaultAttackInfo, 1}});
    addNewWeapon(sekira1, 2, 1773, {{close2LineRange, defaultAttackInfo, 4}, {smallTRangeFlip, defaultAttackInfo, 2}});
    addNewWeapon(sekira1, 3, 1774, {{close2LineRange, defaultAttackInfo, 5}, {smallTRangeFlip, defaultAttackInfo, 3}});

    addNewWeapon(whip, 1, 1775, {{bigIRange, defaultAttackInfo, 2}});
    addNewWeapon(whip, 3, 1806, {{veryBigIRange, defaultAttackInfo, 6}});

    addNewWeapon(bat, 1, 1776, {{smallTRangeFlip, defaultAttackInfo, 2}});

    addNewWeapon(dirk, 1, 1777, {{simpleRange, defaultAttackInfo, 3}});
    addNewWeapon(dirk, 2, 1782, {{simpleRange, defaultAttackInfo, 4}});
    addNewWeapon(dirk, 3, 1783, {{smallIRange, defaultAttackInfo, 5}});

    addNewWeapon(kingsword, 4, 1781, {{ARange, defaultAttackInfo, 6}});

    addNewWeapon(morgenstern1, 1, 1784, {{smallTRange, defaultAttackInfo, 2}});
    addNewWeapon(morgenstern1, 2, 1785, {{smallTRange, defaultAttackInfo, 4}});

    addNewWeapon(bigSekira1, 3, 1786, {{smallTRangeFlip, defaultAttackInfo, 4}, {close2LineRange, defaultAttackInfo, 2}});
    addNewWeapon(bigSekira2, 4, 1787, {{close3LineRange, defaultAttackInfo, 5}});

    addNewWeapon(saber, 1, 1784, {{smallTRangeFlip, defaultAttackInfo, 2}});
    addNewWeapon(saber, 2, 1785, {{bigTRangeFlip, defaultAttackInfo, 3}});

    addNewWeapon(morgenstern2, 1, 1790, {{smallTRange, defaultAttackInfo, 2}});
    addNewWeapon(morgenstern2, 2, 1791, {{XRange, defaultAttackInfo, 4}});

    addNewWeapon(lance, 1, 1794, {{bigIRange, defaultAttackInfo, 2}});
    addNewWeapon(lance, 2, 1795, {{veryBigIRange, defaultAttackInfo, 4}});

    addNewWeapon(sword, 2, 1796, {{bigTRangeFlip, defaultAttackInfo, 3}});
    addNewWeapon(sword, 3, 1797, {{bigTRangeFlip, defaultAttackInfo, 3}, {veryBigIRange, defaultAttackInfo, 2}});

    addNewWeapon(morgenstern3, 2, 1798, {{XRange, defaultAttackInfo, 4}});
    addNewWeapon(morgenstern3, 3, 1799, {{XRange, defaultAttackInfo, 6}});

    addNewWeapon(pike, 1, 1800, {{smallTRange, defaultAttackInfo, 2}});
    addNewWeapon(pike, 3, 1801, {{bigTRange, defaultAttackInfo, 5}});

    addNewWeapon(hammer, 1, 1802, {{closeLineRange, defaultAttackInfo, 2}, {simpleRange, defaultAttackInfo, 1}});
    addNewWeapon(hammer, 2, 1803, {{smallTRangeFlip, defaultAttackInfo, 3}});

    addNewWeapon(axe, 1, 1804, {{closeLineRange, defaultAttackInfo, 2}});
    addNewWeapon(axe, 2, 1805, {{closeLineRange, defaultAttackInfo, 3}});

    addNewWeapon(katana, 2, 1808, {{bigTRangeFlip, defaultAttackInfo, 3}});
    addNewWeapon(katana, 3, 1807, {{ARange, defaultAttackInfo, 5}});

    addNewWeapon(knife, 1, 1802, {{simpleRange, defaultAttackInfo, 3}});

    addNewWeapon(sword, 2, 1812, {{smallTRangeFlip, defaultAttackInfo, 3}});
    addNewWeapon(sword, 3, 1813, {{bigTRangeFlip, defaultAttackInfo, 2}, {bigIRange, defaultAttackInfo, 3}});

    addNewWeapon(gigaSword, 4, 1841, {{XRange, defaultAttackInfo, 5}, {bigTRange, defaultAttackInfo, 3}});

    addNewWeapon(trident, 4, 1842, {{XRange, defaultAttackInfo, 5}, {veryBigIRange, defaultAttackInfo, 4}});

    addNewWeapon(scythe, 4, 1849, {{bigTRange, defaultAttackInfo, 3}, {smallTRange, defaultAttackInfo, 5}});
}



void Attacks::addNewWeapon(const std::string& name, int tier, int gid, const RangesInfo& ranges)
{
    m_createWeapons[name].emplace(tier, createCreatedFunc(gid, ranges));
}



std::function<Weapon*(World*)> Attacks::createCreatedFunc(int gid, const RangesInfo& ranges)
{
    return [ranges, gid](World* world)
        {
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
        };
}

