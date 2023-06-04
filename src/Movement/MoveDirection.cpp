#include "MoveDirection.h"

using namespace cocos2d;

MoveDirection::MoveDirection(World* world)
    : m_world(world) { }

void MoveDirection::move(Node* object)
{
    Size tile = m_world->getTileSize();
    
    Point moveStep = Point(m_direction.x * tile.width, m_direction.y * tile.height);
    object->setPosition(object->getPosition() + moveStep);
}

void MoveDirection::setDirection(Vec2 direction)
{
    m_direction = direction;
}
