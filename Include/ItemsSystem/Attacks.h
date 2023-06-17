#pragma once

#include "ItemsSystem/Weapon.h"

class Attacks
{
public:
    using RangesInfo = std::vector<std::tuple<const std::vector<Vec2Int>, std::shared_ptr<AttackInfo>, float>>;

    static Weapon* createDefaultWeapon(World* world) { return instance.m_createDefault(world); }
    
    static Weapon* createWeapon(World* world, const std::string& name, int tier) { return instance.m_createWeapons.at(name).at(tier)(world); }

    static std::vector<std::pair<std::string, int>> getTiers();

private:
    inline static const std::string defaultWeapon = "Default";
    
    static const Attacks instance;
    std::function<Weapon*(World*)> m_createDefault;

    Attacks();
    
    void initWeaponsAndAttacks();
    void addNewWeapon(const std::string& name, int tier, int gid, const RangesInfo& ranges);
    
    std::function<Weapon*(World*)> createCreatedFunc(int gid, const RangesInfo& ranges);

    std::unordered_map<std::string, std::unordered_map<int, std::function<Weapon* (World*)>>> m_createWeapons;

public:
    inline static const std::vector<Vec2Int> simpleRange = {{1, 0}};
    /*
     * - 0 0 0
     * @ X 0 0
     * - 0 0 0
     */

    inline static const std::vector<Vec2Int> closeLineRange = {{1, 1}, {1, 0}, {1, -1}};
    /*
     * - X 0 0
     * @ X 0 0
     * - X 0 0
     */

    inline static const std::vector<Vec2Int> close2LineRange = {{1, 1}, {1, 0}, {1, -1}, {2, 1}, {2, 0}, {2, -1}};
    /*
     * - X X 0
     * @ X X 0
     * - X X 0
     */

    inline static const std::vector<Vec2Int> close3LineRange = {{1, 1}, {1, 0}, {1, -1}, {2, 1}, {2, 0}, {2, -1}, {3, 1}, {3, 0}, {3, -1}};
    /*
     * - X X X
     * @ X X X
     * - X X X
     */

    inline static const std::vector<Vec2Int> bigIRange = {{1, 0}, {2, 0}, {3, 0}};
    /*
     * - 0 0 0
     * @ X X X
     * - 0 0 0
     */

    inline static const std::vector<Vec2Int> veryBigIRange = {{1, 0}, {2, 0}, {3, 0}, {4, 0}};
    /*
     * - 0 0 0 0
     * @ X X X X
     * - 0 0 0 0
     */

    inline static const std::vector<Vec2Int> smallIRange = {{1, 0}, {2, 0}};
    /*
     * - 0 0 0
     * @ X X 0
     * - 0 0 0
     */

    inline static const std::vector<Vec2Int> bigTRange = {{1, 0}, {2, 0}, {3, 1}, {3, 0}, {3, -1}};
    /*
     * - 0 0 X
     * @ X X X
     * - 0 0 X
     */

    inline static const std::vector<Vec2Int> bigTRangeFlip = {{1, 1}, {1, 0}, {1, -1}, {2, 0}, {3, 0}};
    /*
     * - X 0 0
     * @ X X X
     * - X 0 0
     */

    inline static const std::vector<Vec2Int> smallTRange = {{1, 0}, {2, 1}, {2, 0}, {2, -1}};
    /*
     * - 0 X 0
     * @ X X 0
     * - 0 X 0
     */

    inline static const std::vector<Vec2Int> smallTRangeFlip = {{1, 1}, {1, 0}, {1, -1}, {2, 0}};
    /*
     * - X 0 0
     * @ X X 0
     * - X 0 0
     */

    inline static const std::vector<Vec2Int> XRange = {{1, 0}, {2, 1}, {2, 0}, {2, -1}, {3, 0}};
    /*
     * - 0 X 0
     * @ X X X
     * - 0 X 0
     */

    inline static const std::vector<Vec2Int> ARange = {{1, 1}, {1, 0}, {1, -1}, {2, 1}, {2, 0}, {2, -1}, {3, 0}};
    /*
     * - X X 0
     * @ X X X
     * - X X 0
     */
};
