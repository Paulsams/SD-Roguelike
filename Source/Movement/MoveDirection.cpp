#include "Movement/MoveDirection.h"

using namespace cocos2d;

MoveDirection::MoveDirection() { }

void MoveDirection::move(Node* object, World* world)
{
    Size tile = world->getTileSize();
    
    Point moveStep = Point(m_direction.x * tile.width, m_direction.y * tile.height);
    object->setPosition(object->getPosition() + moveStep);
}

void MoveDirection::setDirection(Vec2Int direction)
{
    m_direction = direction;
}
