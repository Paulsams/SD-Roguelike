#pragma once
#include "2d/CCLabel.h"
#include "Stats/IStat.h"
#include "Stats/Modificators/BoundsModificator.h"
#include "ui/UILayout.h"
#include "ui/UILoadingBar.h"

class StatBar final : public cocos2d::ui::Layout
{
public:
    static StatBar* create(cocos2d::ui::LinearLayoutParameter* marginParameter, cocos2d::Size contentSize,
        const std::string& pathToImage, std::shared_ptr<IStat> stat);

    bool postInit(cocos2d::ui::LinearLayoutParameter* marginParameter, cocos2d::Size contentSize,
        const std::string& pathToImage);

private:
    const FunctionHandler<IStat::currentValue, IStat::changedValue, IStat::wantedChangeValue> m_changeStatDelegate;
    static constexpr float coefficientScaleTextSize = 2.5f;
    static constexpr float textSize = 40.0f;

    explicit StatBar(std::shared_ptr<IStat> stat);
    ~StatBar();

    std::string getTextView() const;

    void OnChanged(IStat::currentValue currentValue, IStat::changedValue, IStat::wantedChangeValue);

    std::shared_ptr<IStat> m_stat;
    std::shared_ptr<BoundsModificator> m_boundsStat;
    cocos2d::ui::LoadingBar* m_bar = nullptr;
    cocos2d::Label* m_label = nullptr;
};
