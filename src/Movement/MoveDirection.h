#pragma once
#include "IMovement.h"
#include "math/Vec2.h"
#include "WorldSystem/World.h"

class MoveDirection : public IMovement
{
public:
    explicit MoveDirection(World* world);
    
    void move(cocos2d::Node* object) override;
    void setDirection(cocos2d::Vec2 direction);
private:
    cocos2d::Vec2 m_direction;
    World* m_world;
};
