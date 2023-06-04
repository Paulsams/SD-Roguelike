#pragma once
#include <memory>

#include "PlayerInput.h"
#include "Utils/Direction.h"
#include "Utils/EventsSystem.h"
#include "ItemsSystem/IAttack.h"
#include "Movement/IMovement.h"
#include "GameLoop/IUpdatable.h"
#include "Stats/IHaveStats.h"

class Player : public cocos2d::Node, public IHaveStats, public IUpdatable
{
public:
    static Player* create(std::shared_ptr<IMovement> movement, std::shared_ptr<IAttack> attack);

    void update() override;
    const std::shared_ptr<StatsContainer> get() const override;

    bool init() override;

    EventContainer<> moved;
private:
    FunctionHandler<Direction> m_moveDelegate;

    Player(std::shared_ptr<IMovement> movement, std::shared_ptr<IAttack> attack);
    void move(Direction direction);
    
    PlayerInput m_input;
    cocos2d::Sprite* m_sprite;
    std::shared_ptr<IMovement> m_movement;
    std::shared_ptr<IAttack> m_attack;
    std::shared_ptr<StatsContainer> m_statsContainer;
};
