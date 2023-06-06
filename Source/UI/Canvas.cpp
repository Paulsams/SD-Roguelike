#include "UI/Canvas.h"

#include <utility>

#include "cocos2d.h"
#include "UI/Colors.h"
#include "UI/StepCounter.h"
#include "GameLoop/GameLoop.h"
#include "Stats/StatsContainer.h"
#include "UI/InventoryView.h"
#include "UI/StatBar.h"
#include "ui/UIHBox.h"
#include "ui/UIImageView.h"
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
    const Size contentSize = Director::getInstance()->getWinSize();
    setContentSize(contentSize);

    const auto box = VBox::create({widthRightPanel, contentSize.height});
    box->setPosition({contentSize.width - widthRightPanel, 0.0f});
    box->setBackGroundColorType(Layout::BackGroundColorType::SOLID);
    box->setBackGroundColor(Colors::background);
    this->addChild(box);

    const auto playerBox = HBox::create({widthRightPanel, 100.0f});
    playerBox->setBackGroundColorType(Layout::BackGroundColorType::SOLID);
    playerBox->setBackGroundColor(Colors::midground);
    box->addChild(playerBox);

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
    
    const auto marginFromBar = Margin(0.0f, 10.0f, 10.0f, 10.0f);
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
    box->addChild(stepCounter);

    const auto inventoryView = InventoryView::create(m_player->getInventory(),
        {widthRightPanel, 420.0f} , {10.0f, 10.0f});
    box->addChild(inventoryView);

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
