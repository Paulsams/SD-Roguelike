#pragma once
#include "Accessory.h"
#include "ConsumableItem.h"
#include "ItemsSystem/BaseItem.h"

struct ItemInfo
{
    const std::string name;
    const int gid;
};

class Items
{
public:
    static const ItemInfo healthPotion;

    static ConsumableItem* createConsumable(World* world, const std::string& name)
        { return instance.m_createConsumables.at(name)(world); }

    static Accessory* createAccessory(World* world, const ItemInfo& info)
        { return instance.m_createAccessories.at(info.name)(world); }

    static std::vector<std::string> getConsumables();

private:
    static const Items instance;
    Items();
    
    void initConsumables();
    static std::function<bool (const Player*)> getInteractedFunctionForChangeStat(StatType statType, float changeValueBy);
    
    std::unordered_map<std::string, std::function<Accessory* (World*)>> m_createAccessories;
    std::unordered_map<std::string, std::function<ConsumableItem* (World*)>> m_createConsumables;
};

inline const Items Items::instance = Items();
