#pragma once

#include "IMovement.h"
#include "Utils/Common.h"

class MoveDirection : public IMovement
{
public:
    explicit MoveDirection();
    
    void move(cocos2d::Node* object, World* world) override;
    void setDirection(Vec2Int direction);
private:
    Vec2Int m_direction;
};
