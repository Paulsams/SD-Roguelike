#pragma once
#include "cocos2d.h"
#include "WorldSystem/World.h"

struct IMovement
{
    virtual ~IMovement() = default;
    virtual void move(cocos2d::Node* object, World* world) = 0;
};
