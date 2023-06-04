#include "Canvas.h"

#include <utility>

#include "cocos2d.h"
#include "Colors.h"
#include "StepCounter.h"
#include "GameLoop/GameLoop.h"
#include "ui/UIHBox.h"
#include "ui/UIImageView.h"
#include "ui/UILoadingBar.h"
#include "ui/UIVBox.h"

using namespace cocos2d;
using namespace cocos2d::ui;
using namespace ::ui;

Canvas* Canvas::create(std::shared_ptr<GameLoop> gameLoop)
{
    Canvas* canvas = new (std::nothrow) Canvas(std::move(gameLoop));
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
    playerStatsBox->setBackGroundColor(Color3B::BLUE);
    playerBox->addChild(playerStatsBox);
    playerBox->addChild(playerIconLayout);
    
    const auto marginFromBar = Margin(10.0f, 10.0f, 10.0f, 10.0f);
    const auto marginSizeOffset = Size(marginFromBar.right + marginFromBar.left,
                                       marginFromBar.top + marginFromBar.bottom);
    LinearLayoutParameter* marginParameter = LinearLayoutParameter::create();
    marginParameter->setMargin(marginFromBar);
    marginParameter->setGravity(LinearLayoutParameter::LinearGravity::CENTER_HORIZONTAL);

    const auto playerHpLayout = Layout::create();
    playerHpLayout->setContentSize(Size(playerStatsBox->getContentSize().width, 50.0f) - marginSizeOffset);
    playerHpLayout->setLayoutParameter(marginParameter);
    playerHpLayout->setAnchorPoint(Vec2::ZERO);
    playerStatsBox->addChild(playerHpLayout);

    const auto playerHp = LoadingBar::create("health-bar.png", 70);
    playerHp->ignoreContentAdaptWithSize(false);
    playerHp->setContentSize(playerHpLayout->getContentSize());
    playerHp->setAnchorPoint(Vec2::ZERO);
    playerHpLayout->addChild(playerHp);

    const auto playerManaLayout = Layout::create();
    playerManaLayout->setContentSize(Size(playerStatsBox->getContentSize().width, 50.0f) - marginSizeOffset);
    playerManaLayout->setLayoutParameter(marginParameter);
    playerStatsBox->addChild(playerManaLayout);

    const auto playerMana = LoadingBar::create("health-bar.png", 30);
    playerMana->ignoreContentAdaptWithSize(false);
    playerMana->setContentSize(playerManaLayout->getContentSize());
    playerMana->setAnchorPoint(Vec2::ZERO);
    playerManaLayout->addChild(playerMana);
    
    const auto stepCounter = StepCounter::create(m_gameLoop->currentStep);
    stepCounter->setContentSize({widthRightPanel, 30.0f});
    box->addChild(stepCounter);

    return true;
}

// search({{-1, 0}, {1, 0}, {0, 1}, {0, -1}, {-1, -1}, {1, 1}, {}})
//
// void search(std::vector<Vec2> possibleMove)
// {
//     for (auto possible : possibleMove)
//     {
//         if (x > 0 && x < width - 1 && y > 0 && y < height - 1)
//             curNode.neighbors.push_back(&nodes[(possible.y - 1) * width + (possible.x + 0)]);
//     }
// }

void Canvas::update(float delta)
{
    this->setPosition(this->getScene()->getDefaultCamera()->getPosition());
}

Canvas::Canvas(std::shared_ptr<GameLoop> gameLoop)
    : m_gameLoop(std::move(gameLoop))
{
    this->scheduleUpdate();
}
