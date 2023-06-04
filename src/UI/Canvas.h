#pragma once
#include "StepCounter.h"
#include "2d/CCNode.h"

class Canvas : public cocos2d::Node
{
public:
    static constexpr float widthRightPanel = 300.0f;
    
    static Canvas* create(std::shared_ptr<GameLoop> gameLoop);

    bool init() override;
    void update(float delta) override;

private:
    Canvas(std::shared_ptr<GameLoop> gameLoop);

    std::shared_ptr<GameLoop> m_gameLoop;
    std::shared_ptr<ui::StepCounter> m_stepCounter;
};
