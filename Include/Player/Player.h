#pragma once
#include <memory>

#include "Backpack.h"
#include "PlayerInput.h"
#include "Utils/Direction.h"
#include "Utils/EventsSystem.h"
#include "ItemsSystem/AttackHandler.h"
#include "Movement/IMovement.h"
#include "GameLoop/IUpdatable.h"
#include "Movement/MoveDirection.h"
#include "Stats/IHaveStats.h"
#include "ItemsSystem/IItem.h"

class Player : public cocos2d::Node, public BaseEntity, public IUpdatable
{
public:
    static Player* create();
    
    const std::shared_ptr<IStatsContainer> getStats() const override { return m_statsContainer; }
    void setWorld(World* world) { m_world = world; }
    
    ObservableVector<std::shared_ptr<IItem>>& getInventory() { return m_items; }

    bool init() override;
    
    void update() override;

    EventContainer<> moved;
private:
    FunctionHandler<Direction> m_moveDelegate;

    Player();
    void move(Direction direction);
    
    PlayerInput m_input;
    cocos2d::Sprite* m_sprite;
    std::shared_ptr<MoveDirection> m_movement;
    Backpack m_backpack;
    std::shared_ptr<StatsContainer> m_statsContainer;
    ObservableVector<std::shared_ptr<IItem>> m_items;
    
    World* m_world = nullptr;
};
