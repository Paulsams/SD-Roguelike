#pragma once
#include <string>

#include "2d/CCSprite.h"

struct IItem
{
    virtual ~IItem() = default;
    
    virtual std::string getName() const = 0;
    virtual std::string getDescription() const = 0;
    virtual cocos2d::Rect getRectSpriteInTileset() const = 0;
};
