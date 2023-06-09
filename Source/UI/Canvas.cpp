#include "UI/Canvas.h"

#include <utility>

#include "cocos2d.h"
#include "UI/StepCounter.h"
#include "GameLoop/GameLoop.h"
#include "UI/InventoryView.h"
#include "UI/PlayerItemsOnUI.h"
#include "UI/StatBar.h"
#include "ui/UIHBox.h"
#include "ui/UIImageView.h"
#include "ui/UIRelativeBox.h"
#include "ui/UIVBox.h"

using namespace cocos2d;
using namespace cocos2d::ui; 
using namespace ::ui;

Canvas* Canvas::create(World* world, Player* player, std::shared_ptr<GameLoop> gameLoop)
{
    Canvas* canvas = new (std::nothrow) Canvas(world, player, std::move(gameLoop));
    if (canvas && canvas->init())
    {
        canvas->autorelease();
        return canvas;
    }
    CC_SAFE_DELETE(canvas);
    return nullptr;
}

bool Canvas::init()
{
    const Size winSize = Director::getInstance()->getWinSize();
    setContentSize(winSize);

    const auto rightBox = VBox::create({widthRightPanel, winSize.height});
    rightBox->setPosition({winSize.width - widthRightPanel, 0.0f});
    rightBox->setBackGroundColorType(Layout::BackGroundColorType::SOLID);
    rightBox->setBackGroundColor(Colors::background);
    this->addChild(rightBox);

    const auto playerBox = HBox::create({widthRightPanel, 100.0f});
    playerBox->setBackGroundColorType(Layout::BackGroundColorType::SOLID);
    playerBox->setBackGroundColor(Colors::midground);
    rightBox->addChild(playerBox);

    const auto playerIconLayout = Layout::create();
    playerIconLayout->setContentSize({playerBox->getContentSize().height, playerBox->getContentSize().height});

    const auto playerIconFrame = ImageView::create("IconFrame.png");
    playerIconFrame->ignoreContentAdaptWithSize(false);
    playerIconFrame->setContentSize(playerIconLayout->getContentSize());
    playerIconFrame->setAnchorPoint(Vec2::ZERO);
    playerIconLayout->addChild(playerIconFrame);

    const auto playerIcon = ImageView::create("Player.png");
    playerIcon->ignoreContentAdaptWithSize(false);
    playerIcon->setContentSize(playerIconLayout->getContentSize());
    playerIcon->setAnchorPoint(Vec2::ZERO);
    playerIconLayout->addChild(playerIcon);
    
    const auto playerStatsBox = VBox::create({widthRightPanel - playerIconLayout->getContentSize().width,
        playerBox->getContentSize().height});
    playerStatsBox->setBackGroundColorType(Layout::BackGroundColorType::SOLID);
    playerStatsBox->setBackGroundColor(Colors::backgroundForStatBar);
    playerBox->addChild(playerStatsBox);
    playerBox->addChild(playerIconLayout);

    auto marginFromBar = Margin(0.0f, 10.0f, 10.0f, 10.0f);
    const auto marginSizeOffset = Size(marginFromBar.left + marginFromBar.right,
                                       marginFromBar.top + marginFromBar.bottom);
    LinearLayoutParameter* marginParameter = LinearLayoutParameter::create();
    marginParameter->setMargin(marginFromBar);
    marginParameter->setGravity(LinearLayoutParameter::LinearGravity::CENTER_HORIZONTAL);

    const std::shared_ptr<IStatsContainer> playerStats = m_player->getStats();
    
    std::shared_ptr<IStat> playerHpStat;
    if (playerStats->tryGet(Health, playerHpStat))
    {
        const auto playerHp = StatBar::create(marginParameter,
            Size(playerStatsBox->getContentSize().width - 40.0f, 50.0f) - marginSizeOffset,
            "health-bar.png",
            playerHpStat);
        playerStatsBox->addChild(playerHp);
    }

    std::shared_ptr<IStat> playerManaStat;
    if (playerStats->tryGet(Mana, playerManaStat))
    {
        const auto playerMana = StatBar::create(marginParameter,
            Size(playerStatsBox->getContentSize().width - 40.0f, 50.0f) - marginSizeOffset,
            "health-bar.png",
            playerManaStat);
        playerStatsBox->addChild(playerMana);
    }
    
    const auto stepCounter = StepCounter::create(m_gameLoop->currentStep);
    stepCounter->setContentSize({widthRightPanel, 30.0f});
    rightBox->addChild(stepCounter);

    const Size cellInventorySize = {90.0f, 90.0f};
    const Size padding = {10.0f, 10.0f};

    auto inventoryView = InventoryView::create(m_player->getInventory(),
        {SPELL, WEAPON, ACCESSORY}, Paths::toAllInventoryCell, cellInventorySize, 3, padding);
    inventoryView->setLayoutParameter(marginParameter);
    rightBox->addChild(inventoryView);

    const float cellMargin = 5.0f;
    Margin horizontalMargin = {cellMargin, 0.0f, cellMargin, 0.0f};
    Margin verticalMargin = {0.0f, cellMargin, 0.0f, cellMargin};

    const auto backpackBox = RelativeBox::create({winSize.width - widthRightPanel, heightBackpack});
    backpackBox->setPosition(Vec2::ZERO);
    backpackBox->setBackGroundColorType(Layout::BackGroundColorType::SOLID);
    backpackBox->setBackGroundColor(Colors::background);
    this->addChild(backpackBox);

    const auto leftParameter = RelativeLayoutParameter::create();
    leftParameter->setAlign(RelativeLayoutParameter::RelativeAlign::PARENT_LEFT_CENTER_VERTICAL);
    leftParameter->setMargin({horizontalMargin.left, verticalMargin.top,
                                horizontalMargin.right, verticalMargin.bottom});
    
    Backpack& backpack = m_player->getBackpack();
    auto backpackSpellsView = InventoryView::create(backpack.getObservableSpells(),
        {SPELL}, Paths::toSpellInventoryCell, cellInventorySize, backpack.getObservableSpells().size(), padding);
    backpackSpellsView->setLayoutParameter(leftParameter);
    backpackBox->addChild(backpackSpellsView);

    const auto horizontalParameter = LinearLayoutParameter::create();
    horizontalParameter->setGravity(LinearLayoutParameter::LinearGravity::CENTER_HORIZONTAL);
    horizontalParameter->setMargin(horizontalMargin);

    auto backpackWeaponView = InventoryView::create(backpack.getObservableWeapons(),
    {WEAPON}, Paths::toWeaponInventoryCell, cellInventorySize, 3, padding);
    backpackWeaponView->setLayoutParameter(horizontalParameter);
    
    auto backpackAccessoriesView = InventoryView::create(backpack.getObservableAccessories(),
        {ACCESSORY}, Paths::toAccessoryInventoryCell, cellInventorySize,
        backpack.getObservableAccessories().size(), padding);
    backpackAccessoriesView->setLayoutParameter(horizontalParameter);

    const auto rightParameter = RelativeLayoutParameter::create();
    rightParameter->setAlign(RelativeLayoutParameter::RelativeAlign::PARENT_RIGHT_CENTER_VERTICAL);
    rightParameter->setMargin(verticalMargin);

    const Size sizeForWeaponWithAccessories = {backpackWeaponView->getContentSize().width +
        backpackAccessoriesView->getContentSize().width + cellMargin * 3, backpackBox->getContentSize().height - cellMargin * 2};
    const auto backpackWeaponWithAccessoriesBox = HBox::create(sizeForWeaponWithAccessories);
    backpackWeaponWithAccessoriesBox->setLayoutParameter(rightParameter);
    
    backpackWeaponWithAccessoriesBox->addChild(backpackWeaponView);
    backpackWeaponWithAccessoriesBox->addChild(backpackAccessoriesView);
    
    backpackBox->addChild(backpackWeaponWithAccessoriesBox);
    
    m_allItemsPlayer = std::make_shared<PlayerItemsOnUI>(inventoryView, backpackWeaponView, backpackAccessoriesView, backpackSpellsView);

    return true;
}

void Canvas::update(float delta)
{
    Node::update(delta);
    
    this->setPosition(this->getScene()->getDefaultCamera()->getPosition());
}

Canvas::Canvas(World* world, Player* player, std::shared_ptr<GameLoop> gameLoop)
    : m_world(world)
    , m_player(player)
    , m_gameLoop(std::move(gameLoop))
{
    this->scheduleUpdate();
}
