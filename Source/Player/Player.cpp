#include "Player/Player.h"

#include "Stats/Modificators/BoundsModificator.h"
#include "Stats/Modificators/StatWithModificators.h"

using namespace cocos2d;

Player* Player::create(World* world)
{
    Player* player = new (std::nothrow) Player(world);
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
    static const Size size = {32, 32};

    constexpr int templatePeopleId = 2210 - 1;
    constexpr int clothesId = 2149 - 1;
    constexpr int helmetId = 1957 - 1;
    
    m_templatePeople = Sprite::create(Paths::toGameTileset);
    m_templatePeople->setTextureRect(Rect(Vec2{templatePeopleId % 64, templatePeopleId / 64} * size.width, size));

    m_clothes = Sprite::create(Paths::toGameTileset);
    m_clothes->setTextureRect(Rect(Vec2{clothesId % 64, clothesId / 64} * size.width, size));
    
    m_helmet = Sprite::create(Paths::toGameTileset);
    m_helmet->setTextureRect(Rect(Vec2{helmetId % 64, helmetId / 64} * size.width, size));
    
    if (!m_helmet || !m_clothes || !m_templatePeople)
    {
        log("The player couldn't find the sprite");
        return false;
    }
    
    m_templatePeople->setAnchorPoint(Vec2::ZERO);
    Node::addChild(m_templatePeople, 4);

    m_clothes->setAnchorPoint(Vec2::ZERO);
    Node::addChild(m_clothes, 3);
    
    m_helmet->setAnchorPoint(Vec2::ZERO);
    Node::addChild(m_helmet, 2);

    return true;
}

Player::Player(World* world)
    : BaseEntity(world)
    , m_moveDelegate(CC_CALLBACK_1(Player::move, this))
    , m_input(this)
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
    Vec2Int newPosition = getPositionInWorld() + direction.getVector();
    TileType tileType = getWorld()->getTileType(newPosition);
    if (tileType == TileType::GROUND)
        setPositionInWorld(newPosition);
}
