#pragma once
#include <memory>

#include "PlayerInput.h"
#include "Utils/Direction.h"
#include "Utils/EventsSystem.h"
#include "Commands/IAttackCommand.h"
#include "Commands/IMoveCommand.h"
#include "GameLoop/IUpdatable.h"
#include "Stats/IHaveStats.h"

class Player : public cocos2d::Node, public IHaveStats, public IUpdatable
{
public:
    static Player* create(std::shared_ptr<IMoveCommand> moveCommand, std::shared_ptr<IAttackCommand> attackCommand);

    void update() override;
    const std::shared_ptr<StatsContainer> get() const override;

    bool init() override;

    EventContainer<> moved;
private:
    FunctionHandler<Direction> moveDelegate = CC_CALLBACK_1(Player::move, this);

    Player(std::shared_ptr<IMoveCommand> moveCommand, std::shared_ptr<IAttackCommand> attackCommand);
    void move(Direction direction);
    
    PlayerInput m_input;
    cocos2d::Sprite* m_sprite;
    std::shared_ptr<IMoveCommand> m_moveCommand;
    std::shared_ptr<IAttackCommand> m_attackCommand;
    std::shared_ptr<StatsContainer> m_stats_container;
};
