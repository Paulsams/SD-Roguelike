#pragma once

#include "Utils/Direction.h"
#include "Utils/EventsSystem.h"

#include "cocos2d.h"


class Player;

/**
 * Player input class
 *
 * Manages all input of the player
 */
class PlayerInput
{
public:
    using attackDelegate = std::function<void()>;
    using directionDelegate = std::function<void(Direction)>;
    using interactDelegate = std::function<void()>;

    explicit PlayerInput(Player* player);

    EventContainer<Direction> moved; /// player move events
    EventContainer<> attacked; /// player attack events
    EventContainer<> interacted; /// player interaction events
private:
    void onKeyPressed(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event) const;
    void tryInvokeDirectionFromInput(cocos2d::EventKeyboard::KeyCode keyCode) const;
};
