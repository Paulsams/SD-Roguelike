#include "Player/PlayerInput.h"
#include "Player/Player.h"

using namespace cocos2d;

PlayerInput::PlayerInput(Player* player)
{
    const auto listener = EventListenerKeyboard::create();
    listener->onKeyPressed = CC_CALLBACK_2(PlayerInput::onKeyPressed, this);
    
    player->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, player);
}

void PlayerInput::onKeyPressed(EventKeyboard::KeyCode keyCode, Event* event) const
{
    tryInvokeDirectionFromInput(keyCode);
    
    switch (keyCode)
    {
    case EventKeyboard::KeyCode::KEY_SPACE:
        attacked();
        break;
    case EventKeyboard::KeyCode::KEY_E:
        interacted();
        break;
    default:
        break;
    }
}

void PlayerInput::tryInvokeDirectionFromInput(const EventKeyboard::KeyCode keyCode) const
{
    switch (keyCode)
    {
    case EventKeyboard::KeyCode::KEY_LEFT_ARROW:
        return moved(Direction{LEFT});
    case EventKeyboard::KeyCode::KEY_UP_ARROW:
        return moved(Direction{UP});
    case EventKeyboard::KeyCode::KEY_RIGHT_ARROW:
        return moved(Direction{RIGHT});
    case EventKeyboard::KeyCode::KEY_DOWN_ARROW:
        return moved(Direction{DOWN});
    default: ;
    }
}
