#include "UI/StatBar.h"

#include <Utils/FontsTTF.h>

#include "2d/CCLabel.h"
#include "Stats/Modificators/BoundsModificator.h"
#include "UI/Colors.h"
#include "ui/UILoadingBar.h"

using namespace cocos2d;
using namespace cocos2d::ui;

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

bool StatBar::postInit(LinearLayoutParameter* marginParameter, Size contentSize, const std::string& pathToImage)
{
    if (!m_stat->tryGet(m_boundsStat))
        return false;
    
    setContentSize(contentSize);
    
    setLayoutParameter(marginParameter);
    setAnchorPoint(Vec2::ZERO);

    setBackGroundColorType(BackGroundColorType::SOLID);
    setBackGroundColor(Colors::backgroundForStat);

    m_bar = LoadingBar::create(pathToImage, m_stat->getValue());
    m_bar->ignoreContentAdaptWithSize(false);
    m_bar->setContentSize(contentSize);
    m_bar->setAnchorPoint(Vec2::ZERO);
    this->addChild(m_bar);

    m_label = Label::createWithTTF(getTextView(), FontsTTF::onUI, 20);
    m_label->setContentSize(contentSize);
    m_label->setPosition(contentSize / 2);
    this->addChild(m_label);

    m_stat->changed += m_changeStatDelegate;

    return true;
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

void StatBar::OnChanged(IStat::oldValue, IStat::currentValue currentValue, IStat::changedValue)
{
    m_bar->setPercent(m_boundsStat->getValueFromPercent(currentValue));
    m_label->setString(getTextView());
}
