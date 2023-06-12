#pragma once

#include "ItemsSystem/Weapon.h"

struct WeaponInfo
{
    const int gid;
    const int tier;
};

class Attacks
{
public:
    static const WeaponInfo defaultWeapon;
    static const WeaponInfo stick_1;
    static const WeaponInfo axe_1;

    static Weapon* createWeapon(World* world, WeaponInfo info)
        { return instance.m_createWeapons.at(info.gid).at(info.tier)(world); }

private:
    static const Attacks instance;
    Attacks();
    
    void initWeaponsAndAttacks();
    void addNewWeapon(WeaponInfo info, std::vector<std::tuple<const std::vector<Vec2Int>,
        std::shared_ptr<AttackInfo>, float>> ranges);

    std::map<int, std::map<int, std::function<Weapon* (World*)>>> m_createWeapons;
};

inline const Attacks Attacks::instance = Attacks();
