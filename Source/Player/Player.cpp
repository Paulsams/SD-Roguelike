#include "Player/Player.h"

#include "Movement/MoveDirection.h"
#include "Stats/Modificators/BoundsModificator.h"
#include "Stats/Modificators/StatWithModificators.h"

using namespace cocos2d;

Player* Player::create()
{
    Player* player = new (std::nothrow) Player();
    if (player && player->init())
    {
        player->autorelease();
        return player;
    }
    CC_SAFE_DELETE(player);
    return nullptr;
}

void Player::update() { }

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

Player::Player()
    : m_moveDelegate(CC_CALLBACK_1(Player::move, this))
    , m_input(this)
    , m_sprite(Sprite::create("Player.png"))
    , m_movement(std::make_shared<MoveDirection>())
    , m_statsContainer(std::make_shared<StatsContainer>())
{
    m_input.moved += m_moveDelegate;

    const auto playerHpStat = std::make_shared<StatWithModificators>(100.0f);
    playerHpStat->addModificator(std::make_shared<BoundsModificator>(MinMax(0, 100.0f)));
    m_statsContainer->add(Health, playerHpStat);

    const auto playerManaStat = std::make_shared<StatWithModificators>(30.0f);
    playerManaStat->addModificator(std::make_shared<BoundsModificator>(MinMax(0, 100.0f)));
    m_statsContainer->add(Mana, playerManaStat);

    m_statsContainer->add(Level, std::make_shared<StatWithModificators>(0));

    for (int i = 0; i < 12; ++i)
        m_items.push_back(nullptr);
}

void Player::move(Direction direction)
{
    m_movement->setDirection(direction.getVector());
    m_movement->move(this, m_world);

    moved();
}
