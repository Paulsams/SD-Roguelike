#pragma once
#include "cocos2d.h"

struct IMoveCommand
{
    virtual ~IMoveCommand() = default;
    virtual void move(cocos2d::Node* object) = 0;
};
