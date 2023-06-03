#pragma once
#include "cocos2d.h"

struct IWorldBuilder
{
    virtual ~IWorldBuilder() = default;
    virtual cocos2d::TMXTiledMap* generate() = 0;
};
