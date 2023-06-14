#include "UI/StatBar.h"

#include <Utils/FontsTTF.h>

#include "2d/CCLabel.h"
#include "Stats/Modificators/BoundsModificator.h"
#include "ui/UIImageView.h"
#include "ui/UILoadingBar.h"

using namespace cocos2d;
using namespace cocos2d::ui;

bool StatBar::postInit(LinearLayoutParameter* marginParameter, Size contentSize, const std::string& pathToImage)
{
    if (!m_stat->tryGet(m_boundsStat))
        return false;
    
    setContentSize(contentSize);
    
    setLayoutParameter(marginParameter);
    setAnchorPoint(Vec2::ZERO);

    ImageView* frame = ImageView::create("FrameStatBar.png");
    frame->ignoreContentAdaptWithSize(false);
    frame->setContentSize(getContentSize());
    frame->setAnchorPoint(Vec2::ZERO);
    this->addChild(frame, 0);

    m_bar = LoadingBar::create(pathToImage, m_boundsStat->getValueFromPercent(m_stat->getValue()));
    m_bar->ignoreContentAdaptWithSize(false);
    m_bar->setContentSize(contentSize);
    m_bar->setAnchorPoint(Vec2::ZERO);
    this->addChild(m_bar, 1);
    
    m_label = Label::createWithTTF(getTextView(), FontsTTF::onUI, textSize);
    m_label->setContentSize(contentSize);
    m_label->setPosition(contentSize / 2);
    m_label->setScale(contentSize.height / textSize / coefficientScaleTextSize);
    this->addChild(m_label, 2);

    m_stat->changed += m_changeStatDelegate;

    return true;
}

StatBar* StatBar::create(LinearLayoutParameter* marginParameter, Size contentSize,
                         const std::string& pathToImage, std::shared_ptr<IStat> stat)
{
    StatBar* statBar = new (std::nothrow) StatBar(stat);
    
    if (statBar && statBar->Layout::init() && statBar->postInit(marginParameter, contentSize, pathToImage))
    {
        statBar->autorelease();
        return statBar;
    }
    CC_SAFE_DELETE(statBar);
    return nullptr;
}

StatBar::StatBar(std::shared_ptr<IStat> stat)
    : m_changeStatDelegate(CC_CALLBACK_3(StatBar::OnChanged, this))
    , m_stat(std::move(stat)) { }

StatBar::~StatBar()
{
    m_stat->changed -= m_changeStatDelegate;
}

std::string StatBar::getTextView() const
{
    return std::to_string(static_cast<int>(m_stat->getValue())) + " / " +
        std::to_string(static_cast<int>(m_boundsStat->bounds.getMax()));
}

void StatBar::OnChanged(IStat::currentValue currentValue, IStat::changedValue, IStat::wantedChangeValue)
{
    m_bar->setPercent(m_boundsStat->getValueFromPercent(currentValue));
    m_label->setString(getTextView());
}
