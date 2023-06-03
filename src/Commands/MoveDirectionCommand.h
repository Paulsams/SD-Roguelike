#pragma once
#include "IMoveCommand.h"
#include "math/Vec2.h"
#include "WorldSystem/World.h"

class MoveDirectionCommand : public IMoveCommand
{
public:
    explicit MoveDirectionCommand(World* world);
    
    void move(cocos2d::Node* object) override;
    void setDirection(cocos2d::Vec2 direction);
private:
    cocos2d::Vec2 m_direction;
    World* m_world;
};
