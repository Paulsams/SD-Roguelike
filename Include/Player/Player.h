#pragma once
#include <memory>

#include "Backpack.h"
#include "PlayerInput.h"
#include "Utils/Direction.h"
#include "Utils/EventsSystem.h"
#include "ItemsSystem/AttackHandler.h"
#include "GameLoop/IUpdatable.h"
#include "Stats/IHaveStats.h"
#include "ItemsSystem/IItem.h"

class Player : public BaseEntity, public IUpdatable
{
public:
    static Player* create(World* world);
    
    const std::shared_ptr<IStatsContainer> getStats() const override { return m_statsContainer; }
    
    ObservableVector<std::shared_ptr<IItem>>& getInventory() { return m_items; }

    bool init() override;
    
    void update() override;

private:
    FunctionHandler<Direction> m_moveDelegate;

    Player(World* world);
    void move(Direction direction);
    
    PlayerInput m_input;
    cocos2d::Sprite* m_helmet = nullptr;
    cocos2d::Sprite* m_clothes = nullptr;
    cocos2d::Sprite* m_templatePeople = nullptr;
    Backpack m_backpack;
    std::shared_ptr<StatsContainer> m_statsContainer;
    ObservableVector<std::shared_ptr<IItem>> m_items;
};
