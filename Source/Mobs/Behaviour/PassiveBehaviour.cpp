#include "Mobs/Behaviour/PassiveBehaviour.h"
#include "Utils/Direction.h"

namespace mob {

    void PassiveBehaviour::update(Mob* mob) {
        auto mobPos = mob->getPositionInWorld();
        DirectionType direction = static_cast<DirectionType>(cocos2d::RandomHelper::random_int(0, 3));
        if (auto possiblePos = mobPos + Direction(direction).getVector();
                mob->getWorld()->getTileType(possiblePos) == TileType::GROUND) {
            mob->setPosition(possiblePos);
        }
    }

}