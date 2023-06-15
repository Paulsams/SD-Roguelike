#include "Player/Player.h"
#include "Stats/Modificators/BoundsModificator.h"
#include "Stats/Modificators/StatWithModificators.h"
#include "WorldSystem/Chest.h"

#include <optional>

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

Player::~Player()
{
    const std::vector<BaseItem*> items = m_items->getCollection();
    for (int i = 0; i < items.size(); ++i)
    {
        if (items[i])
        {
            m_items->setAt(i, nullptr);
            items[i]->release();
        }
    }
}

void Player::throwAllItems()
{
    const std::vector<BaseItem*> items = m_items->getCollection();
    for (int i = 0; i < items.size(); ++i)
    {
        if (items[i])
        {
            m_items->setAt(i, nullptr);
            items[i]->throwOff();
        }
    }
    m_backpack.throwAllItems();
}

bool Player::init()
{
    createClothe(HELMET);
    createClothe(BIB);
    createClothe(TEMPLATE_PEOPLE);

    return true;
}

void Player::update()
{
    m_statsContainer->get(MANA)->changeValueBy(1);
    scheduleDamageIndicators();
}

void Player::setClothe(ClotheType type, const SpriteWithRect& textureInfo)
{
    Sprite* clothe = m_clothes.at(type);
    clothe->setTexture(textureInfo.first);
    clothe->setTextureRect(textureInfo.second);
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
    static std::vector<Direction> directions;
    
    if (m_choicedDirection.has_value())
    {
        directions.clear();
        directions.push_back(m_choicedDirection.value());
        
        DamageIndicatorsSystems* damageIndicators = getWorld()->getDamageIndicatorsForPlayer(this);
        if (const Weapon* currentWeapon = m_backpack.getCurrentWeapon())
            currentWeapon->scheduleDrawIndicators(damageIndicators, this, directions);
        
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

Player::Player(World* world)
    : BaseEntity(world)
    , m_moveDelegate(CC_CALLBACK_1(Player::onMove, this))
    , m_attackedInputDelegate(CC_CALLBACK_0(Player::onAttacked, this))
    , m_interactedDelegate(CC_CALLBACK_0(Player::onInteracted, this))
    , m_input(this)
    , m_backpack(Attacks::createDefaultWeapon(world))
    , m_statsContainer(std::make_shared<StatsContainer>())
    , m_items(std::make_shared<ObservableVector<BaseItem*>>(12))
    , m_attackedDelegate([this](BaseEntity* entity, float damage)
    {
        std::shared_ptr<IStat> healthMobStat;
        if (entity->getStats()->tryGet(HEALTH, healthMobStat) &&
            std::max(healthMobStat->getValue() - damage, 0.0f) == 0.0f)
                entity->acceptVisit(m_killVisitor);
    })
{
    m_input.moved += m_moveDelegate;
    m_input.attacked += m_attackedInputDelegate;
    m_input.interacted += m_interactedDelegate;

    m_backpack.changedCurrentWeapon += [this] { scheduleDamageIndicators(true); };

    m_killVisitor = FunctionVisitorEntitiesBuilder<void>()
        .setMob([this](const mob::Mob* mob)
        {
            m_levelHandler->changeExperiencePoints(mob->getExperiencePoints());
        })
        .build();

    const auto playerHpStat = std::make_shared<StatWithModificators>(100.0f);
    playerHpStat->addModificator(std::make_shared<BoundsModificator>(MinMax(0, 100.0f)));
    m_statsContainer->add(HEALTH, playerHpStat);

    const auto playerManaStat = std::make_shared<StatWithModificators>(30.0f);
    playerManaStat->addModificator(std::make_shared<BoundsModificator>(MinMax(0, 100.0f)));
    m_statsContainer->add(MANA, playerManaStat);

    m_levelHandler = std::make_shared<LevelHandler>(m_statsContainer,
        std::vector<LevelUpInfo>{
            {20, 10},
            {40, 15},
            {60, 30},
            {100, 20},
            {150, 30},
            {200, 50},
            {300, 50}
        },
        [this](const LevelUpInfo& info)
        {
            const std::shared_ptr<IStat> healthStat = m_statsContainer->get(HEALTH);
            std::shared_ptr<BoundsModificator> boundsModificator;
            healthStat->tryGet(boundsModificator);
            boundsModificator->bounds = MinMax(boundsModificator->bounds.getMin(),
                boundsModificator->bounds.getMax() + info.hpIncrease);
            healthStat->changeValueBy(info.hpIncrease);
        });
}

void Player::onAttacked()
{
    Weapon* currentWeapon = m_backpack.getCurrentWeapon();
    if (!currentWeapon || !m_choicedDirection.has_value())
        return;

    currentWeapon->attacked += m_attackedDelegate;
    currentWeapon->attack(getPositionOnMap(), m_choicedDirection.value());
    currentWeapon->attacked -= m_attackedDelegate;

    attacked();
}

void Player::onInteracted()
{
    const auto itemVisitor = FunctionVisitorEntitiesBuilder<void>()
        .setItem([this](BaseItem* item)
        {
            const std::vector<BaseItem*> items = m_items->getCollection();
            const size_t insertIndex = std::ranges::find(items, nullptr) - items.begin();
            if (insertIndex < m_items->size())
            {
                m_items->setAt(insertIndex, item);
                item->pickUp(this);
            }
        })
        .build();

    // В идеале тут надо было бы делать интерфейс IInteractable какой-нибудь, но это бы переусложнило систему и пришлось
    // бы делать много динамик кастов (аля компонентная система, но через полиморфизм), но так как у нас только
    // определенные типы взаимодействий возможны -- сделал пока что так.
    const auto chestVisitor = FunctionVisitorEntitiesBuilder<void>()
        .setChest([this](Chest* chest)
        {
            chest->interact();
        })
        .build();

    if (m_choicedDirection.has_value())
    {
        const Vec2Int checkPosition = getPositionOnMap() + m_choicedDirection.value().getVector();
        for (BaseEntity* entity : getWorld()->getEntitiesFromCell(checkPosition))
        {
            entity->acceptVisit(chestVisitor);
            if (chestVisitor->isCalled())
                return;
        }
    }

    // TODO: оставить только верхний интеракт, а предметы на которых стоишь показывать через ещё одно окошко инвентаря
    for (BaseEntity* entity : getWorld()->getEntitiesFromCell(getPositionOnMap()))
    {
        entity->acceptVisit(itemVisitor);
        if (itemVisitor->isCalled())
            return;
    }
}
