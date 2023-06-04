#pragma once
#include "cocos2d.h"

struct IMovement
{
    virtual ~IMovement() = default;
    virtual void move(cocos2d::Node* object) = 0;
};
