#include <optional>

#include "Player/Player.h"
#include "Stats/Modificators/BoundsModificator.h"
#include "Stats/Modificators/StatWithModificators.h"

using namespace cocos2d;

Player* Player::create(World* world)
{
    auto* player = new (std::nothrow) Player(world);
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
    createClothe(Helmet);
    createClothe(Bib);
    createClothe(TemplatePeople);

    return true;
}

void Player::update()
{
    m_statsContainer->get(Mana)->changeValueBy(1);
    scheduleDamageIndicators();
}

void Player::setClothe(ClotheType type, const SpriteWithRect& textureInfo)
{
    Sprite* clothe = m_clothes.at(type);
    clothe->setTexture(textureInfo.first);
    clothe->setTextureRect(textureInfo.second);
}

Player::Player(World* world)
    : BaseEntity(world)
    , m_moveDelegate(CC_CALLBACK_1(Player::onMove, this))
    , m_attackedDelegate(CC_CALLBACK_0(Player::onAttacked, this))
    , m_interactedDelegate(CC_CALLBACK_0(Player::onInteracted, this))
    , m_input(this)
    , m_backpack(Attacks::createWeapon(world, Attacks::defaultWeapon))
    , m_statsContainer(std::make_shared<StatsContainer>())
    , m_items(12)
{
    m_input.moved += m_moveDelegate;
    m_input.attacked += m_attackedDelegate;
    m_input.interacted += m_interactedDelegate;

    m_backpack.changedCurrentWeapon += [this] { scheduleDamageIndicators(true); };

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

void Player::createClothe(ClotheType type)
{
    Sprite* clothe = Sprite::create();
    clothe->setAnchorPoint(Vec2::ZERO);
    Node::addChild(clothe, type);
    m_clothes.insert({type, clothe});
}

void Player::scheduleDamageIndicators(bool isForcedUpdate) const
{
    if (m_choicedDirection.has_value())
    {
        DamageIndicatorsSystems* damageIndicators = getWorld()->getDamageIndicatorsForPlayer(this);
        if (const Weapon* currentWeapon = m_backpack.getCurrentWeapon())
            currentWeapon->scheduleDrawIndicators(damageIndicators, getPositionOnMap(), m_choicedDirection.value());
        
        if (isForcedUpdate)
            damageIndicators->update();
    }
}

void Player::onMove(Direction direction)
{
    // TODO: написать визитёра, который просто чекает, что вызвался ли метод, чтобы не писать вот такие пустые лямбды
    static std::shared_ptr<FunctionVisitorEntitiesReturnVoid> visitor = FunctionVisitorEntitiesBuilder<void>()
        .setDecoration([](Decoration*) { })
        .setMob([](mob::Mob*){ })
        .setChest([](Chest*){ })
        .setPlayer([](Player*) { }).build();
    
    if (m_choicedDirection.has_value() && m_choicedDirection.value() == direction)
    {
        Vec2Int newPosition = getPositionOnMap() + direction.getVector();
        TileType tileType = getWorld()->getTileType(newPosition);
        if (tileType == TileType::GROUND)
        {
            for (BaseEntity* entity : getWorld()->getEntitiesFromCell(newPosition))
            {
                entity->acceptVisit(visitor);
                if (visitor->isCalled())
                    return;
            }
            setScheduleMovePositionOnMap(newPosition);
        }
        return;
    }
    
    m_choicedDirection.emplace(direction);
    scheduleDamageIndicators(true);
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
