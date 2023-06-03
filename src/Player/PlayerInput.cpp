#include "PlayerInput.h"
#include "Player.h"

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
    case EventKeyboard::KeyCode::KEY_X:
        attacked();
        break;
    default: ;
    }
}

void PlayerInput::tryInvokeDirectionFromInput(const EventKeyboard::KeyCode keyCode) const
{
    switch (keyCode)
    {
    case EventKeyboard::KeyCode::KEY_LEFT_ARROW:
        return moved(Direction{Left});
    case EventKeyboard::KeyCode::KEY_UP_ARROW:
        return moved(Direction{Up});
    case EventKeyboard::KeyCode::KEY_RIGHT_ARROW:
        return moved(Direction{Right});
    case EventKeyboard::KeyCode::KEY_DOWN_ARROW:
        return moved(Direction{Down});
    default: ;
    }
}
