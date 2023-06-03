#include "Player.h"

#include "Commands/MoveDirectionCommand.h"

using namespace cocos2d;

Player* Player::create(std::shared_ptr<IMoveCommand> moveCommand, std::shared_ptr<IAttackCommand> attackCommand)
{
    Player* player = new (std::nothrow) Player(std::move(moveCommand), std::move(attackCommand));
    if (player && player->init())
    {
        player->autorelease();
        return player;
    }
    CC_SAFE_DELETE(player);
    return nullptr;
}

void Player::update() { }

const std::shared_ptr<StatsContainer> Player::get() const
{
    return m_stats_container;
}

bool Player::init()
{
    if (m_sprite == nullptr)
    {
        log("The player couldn't find the sprite");
        return false;
    }

    Node::addChild(m_sprite);

    return true;
}

Player::Player(std::shared_ptr<IMoveCommand> moveCommand, std::shared_ptr<IAttackCommand> attackCommand)
    : m_input(this)
    , m_sprite(Sprite::create("Player.png"))
    , m_moveCommand(std::move(moveCommand))
    , m_attackCommand(std::move(attackCommand))
    , m_stats_container(std::make_shared<StatsContainer>())
{
    m_input.moved += moveDelegate;
}

void Player::move(Direction direction)
{
    std::dynamic_pointer_cast<MoveDirectionCommand>(m_moveCommand)->setDirection(direction.getVector());
    m_moveCommand->move(this);

    moved();
}
