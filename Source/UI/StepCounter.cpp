#include "UI/StepCounter.h"

#include "2d/CCLabel.h"
#include "Utils/FontsTTF.h"

ui::StepCounter* ui::StepCounter::create(ValueNotifyChanged<uint32_t>& step)
{
    StepCounter* stepCounter = new (std::nothrow) StepCounter(step);
    
    if (stepCounter && stepCounter->init())
    {
        stepCounter->autorelease();
        return stepCounter;
    }
    CC_SAFE_DELETE(stepCounter);
    return nullptr;
}

bool ui::StepCounter::init()
{
    cocos2d::Label* label = cocos2d::Label::createWithTTF(formatOutputString(0),
        FontsTTF::onUI, 24);
    
    label->setAnchorPoint(cocos2d::Vec2::ZERO);
    this->addChild(label);
    
    m_step.changed += [label](uint32_t value)
    {
        label->setString(formatOutputString(value));
    };

    return true;
}

std::string ui::StepCounter::formatOutputString(uint32_t value)
{
    return "Current Step:" + std::to_string(value);
}

ui::StepCounter::StepCounter(ValueNotifyChanged<uint32_t>& step)
    : m_step(step) { }
