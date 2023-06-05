#pragma once
#include <memory>

#include "PlayerInput.h"
#include "Utils/Direction.h"
#include "Utils/EventsSystem.h"
#include "ItemsSystem/IAttack.h"
#include "Movement/IMovement.h"
#include "GameLoop/IUpdatable.h"
#include "Movement/MoveDirection.h"
#include "Stats/IHaveStats.h"

class Player : public cocos2d::Node, public IHaveStats, public IUpdatable
{
public:
    static Player* create();
    
    const std::shared_ptr<StatsContainer> getStats() const override { return m_statsContainer; }
    void setWorld(World* world) { m_world = world; }

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
    std::shared_ptr<IAttack> m_attack;
    std::shared_ptr<StatsContainer> m_statsContainer;
    
    World* m_world = nullptr;
};
