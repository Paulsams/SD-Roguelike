#pragma once

#include "ItemsSystem/Weapon.h"

class Attacks
{
public:
    static const std::string defaultWeapon;

    static Weapon* createWeapon(World* world, const std::string& name, const int tier)
        { return instance.m_createWeapons.at(name).at(tier)(world); }

    static std::map<int, std::vector<std::string>> getTiers();

private:
    static const Attacks instance;
    Attacks();
    
    void initWeaponsAndAttacks();
    void addNewWeapon(const std::string& name, int tier, int gid,
        std::vector<std::tuple<const std::vector<Vec2Int>, std::shared_ptr<AttackInfo>, float>> ranges);

    std::unordered_map<std::string, std::unordered_map<int, std::function<Weapon* (World*)>>> m_createWeapons;
};
