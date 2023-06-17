#include "ItemsSystem/Items.h"

#include "Player/Player.h"

std::vector<std::string> Items::getConsumables()
{
    std::vector<std::string> result;
    for (auto [name, createFunc] : instance.m_createConsumables)
        result.push_back(name);
    return result;
}

Items::Items()
{
    initConsumables();
}

void Items::initConsumables()
{
    m_createConsumables.insert({"Apple", [](World* world)
    {
        return ConsumableItem::create(world, world->getRectFromGid(1500),
                                      getInteractedFunctionForChangeStat(HEALTH, 5));
    }});

    m_createConsumables.insert({"Grape", [](World* world)
    {
        return ConsumableItem::create(world, world->getRectFromGid(1510),
                                      getInteractedFunctionForChangeStat(HEALTH, 10));
    }});

    m_createConsumables.insert({"Cheese", [](World* world)
    {
        return ConsumableItem::create(world, world->getRectFromGid(1506),
                                      getInteractedFunctionForChangeStat(HEALTH, 15));
    }});

    m_createConsumables.insert({"Meat", [](World* world)
    {
        return ConsumableItem::create(world, world->getRectFromGid(1508),
            getInteractedFunctionForChangeStat(HEALTH, 25));
    }});

    m_createConsumables.insert({"HealthPotion", [](World* world)
    {
        return ConsumableItem::create(world, world->getRectFromGid(1602),
                                      getInteractedFunctionForChangeStat(HEALTH, 50));
    }});
}

std::function<bool (const Player*)> Items::getInteractedFunctionForChangeStat(StatType statType, float changeValueBy)
{
    return [statType, changeValueBy](const Player* player)
    {
        std::shared_ptr<IStat> healthStat;
        if (player->getStats()->tryGet(statType, healthStat))
        {
            healthStat->changeValueBy(changeValueBy);
            return true;
        }

        return false;
    };
}
