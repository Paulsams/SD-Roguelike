#pragma once
#include <cstdint>

#include "ui/UILayout.h"
#include "Utils/ValueNotifyChanged.h"

class GameLoop;

namespace ui
{
    class StepCounter : public cocos2d::ui::Layout
    {
    public:
        static StepCounter* create(ValueNotifyChanged<uint32_t>& step);

        bool init() override;
        
        static std::string formatOutputString(uint32_t value);
    private:
        explicit StepCounter(ValueNotifyChanged<uint32_t>& step);
        
        ValueNotifyChanged<uint32_t>& m_step;
    };
}
