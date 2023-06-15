#include "UI/Canvas.h"

#include <utility>

#include "cocos2d.h"
#include "GameLoop/GameLoop.h"
#include "UI/InventoryView.h"
#include "UI/PlayerItemsOnUI.h"
#include "UI/StatBar.h"
#include "ui/UIButton.h"
#include "ui/UIHBox.h"
#include "ui/UIRelativeBox.h"
#include "ui/UIVBox.h"
#include "Utils/FontsTTF.h"

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
    
    const auto playerStatsBox = VBox::create({widthRightPanel, 180.0f});
    playerStatsBox->setBackGroundColorType(Layout::BackGroundColorType::SOLID);
    playerStatsBox->setBackGroundColor(Colors::backgroundForStatBar);
    rightBox->addChild(playerStatsBox);

    auto marginFromBar = Margin(0.0f, 10.0f, 10.0f, 10.0f);
    const auto marginSizeOffset = Size(marginFromBar.left + marginFromBar.right,
                                       marginFromBar.top + marginFromBar.bottom);
    LinearLayoutParameter* marginParameter = LinearLayoutParameter::create();
    marginParameter->setMargin(marginFromBar);
    marginParameter->setGravity(LinearLayoutParameter::LinearGravity::CENTER_HORIZONTAL);

    const std::shared_ptr<IStatsContainer> playerStats = m_player->getStats();

    const Size sizeStat = Size(playerStatsBox->getContentSize().width - 10.0f,
                               playerStatsBox->getContentSize().height / 3) - marginSizeOffset;
    
    std::shared_ptr<IStat> playerHpStat;
    if (playerStats->tryGet(HEALTH, playerHpStat))
    {
        const auto playerHp = StatBar::create(marginParameter,
            sizeStat,
            Paths::toHealthBar,
            playerHpStat);
        playerStatsBox->addChild(playerHp);
    }

    std::shared_ptr<IStat> playerManaStat;
    if (playerStats->tryGet(MANA, playerManaStat))
    {
        const auto playerMana = StatBar::create(marginParameter,
            sizeStat,
            Paths::toManaBar,
            playerManaStat);
        playerStatsBox->addChild(playerMana);
    }

    std::shared_ptr<IStat> playerLevelStat;
    std::shared_ptr<IStat> playerLevelPointsStat;
    if (playerStats->tryGet(LEVEL, playerLevelStat) &&
        playerStats->tryGet(LEVEL_POINTS, playerLevelPointsStat))
    {
        constexpr float widthLabel = 50.0f;
        
        HBox* levelBox = HBox::create(sizeStat);
        levelBox->setLayoutParameter(marginParameter);
        
        const auto playerLevelPoints = StatBar::create(nullptr,
            {levelBox->getContentSize().width - widthLabel, levelBox->getContentSize().height},
            Paths::toExperiencePointsBar, 
            playerLevelPointsStat);
        levelBox->addChild(playerLevelPoints);

        Label* level = Label::createWithTTF("1", FontsTTF::onUI, 32);
        playerLevelStat->changed += [level](IStat::currentValue currentValue, IStat::changedValue, IStat::wantedChangeValue)
        {
            level->setString(std::to_string(static_cast<int>(currentValue)));
        };
        level->setPosition({playerLevelPoints->getContentSize().width + widthLabel / 2,
             levelBox->getContentSize().height / 2});
        levelBox->addChild(level);

        playerStatsBox->addChild(levelBox);
    }

    const Size cellInventorySize = {90.0f, 90.0f};
    const Size padding = {10.0f, 10.0f};

    auto inventoryView = InventoryView::create(m_player->getInventory(),
        {SPELL, WEAPON, ACCESSORY, CONSUMABLE}, Paths::toAllInventoryCell, cellInventorySize, 3, padding);
    inventoryView->setLayoutParameter(marginParameter);
    rightBox->addChild(inventoryView);

    constexpr float cellMargin = 5.0f;
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
        {SPELL}, Paths::toSpellInventoryCell, cellInventorySize, backpack.getObservableSpells()->size(), padding);
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
        backpack.getObservableAccessories()->size(), padding);
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

void Canvas::showRestartScreen(const std::string& text, const std::function<void()>& restartCallback)
{
    m_allItemsPlayer.reset();

    Layout* layout = Layout::create();
    layout->setContentSize(getContentSize());
    layout->setBackGroundColorType(Layout::BackGroundColorType::SOLID);
    layout->setBackGroundColor(Color3B::BLACK);
    layout->setOpacity(0);
    this->addChild(layout);

    FadeIn* fadeAction = FadeIn::create(2.0f);
    layout->runAction(fadeAction);

    Label* gameOverLabel = Label::createWithTTF(text, FontsTTF::onUI, 40);
    gameOverLabel->setPosition(getContentSize() / 2);
    layout->addChild(gameOverLabel);

    Button* restartButton = Button::create("ButtonNormal.png", "ButtonPressed.png");
    restartButton->setContentSize({300.0f, 300.0f});
    restartButton->setPosition(getContentSize() / 2);
    restartButton->setPositionY(restartButton->getPositionY() - restartButton->getContentSize().height);
    restartButton->addClickEventListener([restartCallback](Ref*) { restartCallback(); });
    restartButton->setTitleFontName(FontsTTF::onUI);
    restartButton->setTitleText("Restart");
    restartButton->setTitleFontSize(32);

    layout->addChild(restartButton);
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
