#pragma once

#include "cocos2d.h"
#include "Utils/Direction.h"
#include "Utils/EventsSystem.h"

class Player;

class PlayerInput
{
public:
    using attackDelegate = std::function<void()>;
    using directionDelegate = std::function<void(Direction)>;
    using interactDelegate = std::function<void()>;

    explicit PlayerInput(Player* player);

    EventContainer<Direction> moved;
    EventContainer<> attacked;
    EventContainer<> interacted;
private:
    void onKeyPressed(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event) const;
    void tryInvokeDirectionFromInput(cocos2d::EventKeyboard::KeyCode keyCode) const;
};
