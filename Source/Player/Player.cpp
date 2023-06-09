#include <optional>

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

void Player::update()
{
    if (m_choicedDirection.has_value())
    {
        DamageIndicatorsSystems* damageIndicators = getWorld()->getDamageIndicatorsForPlayer();
        if (const Weapon* currentWeapon = m_backpack.getCurrentWeapon())
            currentWeapon->drawIndicators(damageIndicators, getPositionOnMap(), m_choicedDirection.value());
    }

    m_statsContainer->get(Mana)->changeValueBy(1);
}

Player::Player(World* world)
    : BaseEntity(world)
    , m_moveDelegate(CC_CALLBACK_1(Player::onMove, this))
    , m_attackedDelegate(CC_CALLBACK_0(Player::onAttacked, this))
    , m_interactedDelegate(CC_CALLBACK_0(Player::onInteracted, this))
    , m_input(this)
    , m_statsContainer(std::make_shared<StatsContainer>())
    , m_items(12)
{
    m_input.moved += m_moveDelegate;
    m_input.attacked += m_attackedDelegate;
    m_input.interacted += m_interactedDelegate;

    const auto playerHpStat = std::make_shared<StatWithModificators>(100.0f);
    playerHpStat->addModificator(std::make_shared<BoundsModificator>(MinMax(0, 100.0f)));
    m_statsContainer->add(Health, playerHpStat);

    const auto playerManaStat = std::make_shared<StatWithModificators>(30.0f);
    playerManaStat->addModificator(std::make_shared<BoundsModificator>(MinMax(0, 100.0f)));
    m_statsContainer->add(Mana, playerManaStat);

    m_statsContainer->add(Level, std::make_shared<StatWithModificators>(0));

    m_interactedVisitor = FunctionVisitorEntitiesBuilder<void>().setItem([this](BaseItem* item)
    {
        item->pickUp(this);
        const std::vector<BaseItem*> items = m_items.getCollection();
        m_items.setAt(std::ranges::find(items, nullptr) - items.begin(), item);
    }).build();
}

void Player::onMove(Direction direction)
{
    if (m_choicedDirection.has_value() && m_choicedDirection.value() == direction)
    {
        Vec2Int newPosition = getPositionOnMap() + direction.getVector();
        TileType tileType = getWorld()->getTileType(newPosition);
        if (tileType == TileType::GROUND)
            setPositionOnMap(newPosition);
        return;
    }

    DamageIndicatorsSystems* damageIndicators = getWorld()->getDamageIndicatorsForPlayer();
    damageIndicators->reset();
    if (const Weapon* currentWeapon = m_backpack.getCurrentWeapon())
        currentWeapon->drawIndicators(damageIndicators, getPositionOnMap(), direction);
    
    m_choicedDirection.emplace(direction);
}

void Player::onAttacked()
{
    const Weapon* currentWeapon = m_backpack.getCurrentWeapon();
    if (!currentWeapon || !m_choicedDirection.has_value())
        return;
    
    currentWeapon->attack(getPositionOnMap(), m_choicedDirection.value());
    attacked();
}

void Player::onInteracted()
{
    for (BaseEntity* entity : getWorld()->getEntitiesFromCell(getPositionOnMap()))
    {
        entity->acceptVisit(m_interactedVisitor);
        if (m_interactedVisitor->isCalled())
            break;
    }
}
