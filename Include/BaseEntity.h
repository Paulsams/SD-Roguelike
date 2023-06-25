#pragma once

#include "math/Vec2.h"

/**
 * Base class for any entity in the game
 */
class BaseEntity
{
protected:
    cocos2d::Vec2 position; /// position in the world
};
