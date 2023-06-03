#pragma once

#include "cocos2d.h"
#include "Utils/Direction.h"
#include "Utils/EventsSystem.h"

class Player;

class PlayerInput
{
public:
    using attack_func_type = std::function<void()>;
    using direction_func_type = std::function<void(Direction)>;

    explicit PlayerInput(Player* player);

    EventContainer<Direction> moved;
    EventContainer<> attacked;
private:
    void onKeyPressed(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event) const;
    void tryInvokeDirectionFromInput(cocos2d::EventKeyboard::KeyCode keyCode) const;
};
