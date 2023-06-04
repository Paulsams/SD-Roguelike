#include "Player/Player.h"

#include "Movement/MoveDirection.h"
#include "Stats/StatProxy.h"
#include "Stats/Modificators/StatWithModificators.h"

using namespace cocos2d;

Player* Player::create(std::shared_ptr<IMovement> movement, std::shared_ptr<IAttack> attack)
{
    Player* player = new (std::nothrow) Player(std::move(movement), std::move(attack));
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
    return m_statsContainer;
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

Player::Player(std::shared_ptr<IMovement> movement, std::shared_ptr<IAttack> attack)
    : m_moveDelegate(CC_CALLBACK_1(Player::move, this))
    , m_input(this)
    , m_sprite(Sprite::create("Player.png"))
    , m_movement(std::move(movement))
    , m_attack(std::move(attack))
    , m_statsContainer(std::make_shared<StatsContainer>())
{
    m_input.moved += m_moveDelegate;
    m_statsContainer->add(Health, std::make_shared<StatWithModificators>());
    m_statsContainer->add(Mana, std::make_shared<StatWithModificators>());
    m_statsContainer->add(Level, std::make_shared<StatWithModificators>());
    // m_statsContainer->add(Attack, std::make_shared<StatProxy>());
}

void Player::move(Direction direction)
{
    std::dynamic_pointer_cast<MoveDirection>(m_movement)->setDirection(direction.getVector());
    m_movement->move(this);

    moved();
}
