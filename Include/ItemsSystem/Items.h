#pragma once

#include "ItemsSystem/Accessory.h"
#include "ItemsSystem/ConsumableItem.h"
#include "ItemsSystem/BaseItem.h"


/**
 * Singleton
 * Holds all consumables and accessories in the game
 */
class Items
{
public:
    /**
     * Create consumable with given name
     * @param world world
     * @param name consumable name
     * @return consumable item
     */
    static ConsumableItem* createConsumable(World* world, const std::string& name)
        { return instance.m_createConsumables.at(name)(world); }

    /**
     * Create accessory with given name
     * @param world world
     * @param name accessory name
     * @return accessory
     */
    static Accessory* createAccessory(World* world, const std::string& name)
        { return instance.m_createAccessories.at(name)(world); }

    /**
     * @return all game consumables
     */
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
