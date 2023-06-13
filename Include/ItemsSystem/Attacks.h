#pragma once

#include "ItemsSystem/Weapon.h"

class Attacks
{
public:
    static Weapon* createDefaultWeapon(World* world)
        { return instance.m_createDefault(world); }
    
    static Weapon* createWeapon(World* world, const std::string& name, const int tier)
        { return instance.m_createWeapons.at(name).at(tier)(world); }

    static std::map<int, std::vector<std::string>> getTiers();

private:
    inline static const std::string defaultWeapon = "Default"; 
    
    static const Attacks instance;
    Attacks();
    
    void initWeaponsAndAttacks();
    void addNewWeapon(const std::string& name, int tier, int gid,
        const std::vector<std::tuple<const std::vector<Vec2Int>, std::shared_ptr<AttackInfo>, float>>& ranges);
    
    std::function<Weapon*(World*)> createCreatedFunc(int gid,
        std::vector<std::tuple<const std::vector<Vec2Int>, std::shared_ptr<AttackInfo>, float>> ranges);

    std::function<Weapon*(World*)> m_createDefault;
    std::unordered_map<std::string, std::unordered_map<int, std::function<Weapon* (World*)>>> m_createWeapons;
};
