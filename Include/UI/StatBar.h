#pragma once
#include "2d/CCLabel.h"
#include "Stats/IStat.h"
#include "Stats/Modificators/BoundsModificator.h"
#include "ui/UILayout.h"
#include "ui/UILoadingBar.h"

class StatBar : public cocos2d::ui::Layout
{
public:
    static StatBar* create(cocos2d::ui::LinearLayoutParameter* marginParameter, cocos2d::Size contentSize,
        const std::string& pathToImage, std::shared_ptr<IStat> stat);

    bool postInit(cocos2d::ui::LinearLayoutParameter* marginParameter, cocos2d::Size contentSize,
        const std::string& pathToImage);

private:
    const FunctionHandler<IStat::oldValue, IStat::currentValue, IStat::changedValue> m_changeStatDelegate;

    explicit StatBar(std::shared_ptr<IStat> stat);
    ~StatBar();

    std::string getTextView() const;

    void OnChanged(IStat::oldValue oldValue, IStat::currentValue currentValue, IStat::changedValue changedValue);

    std::shared_ptr<IStat> m_stat;
    std::shared_ptr<BoundsModificator> m_boundsStat;
    cocos2d::ui::LoadingBar* m_bar = nullptr;
    cocos2d::Label* m_label = nullptr;
};
