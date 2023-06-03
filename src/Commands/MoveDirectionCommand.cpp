#include "MoveDirectionCommand.h"

using namespace cocos2d;

MoveDirectionCommand::MoveDirectionCommand(World* world)
    : m_world(world) { }

void MoveDirectionCommand::move(Node* object)
{
    Size tile = m_world->getTileSize();
    
    Point moveStep = Point(m_direction.x * tile.width, m_direction.y * tile.height);
    object->setPosition(object->getPosition() + moveStep);
}

void MoveDirectionCommand::setDirection(Vec2 direction)
{
    m_direction = direction;
}
