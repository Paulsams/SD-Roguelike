#include "Mobs/Behaviour/AggressiveBehaviour.h"
#include "Player/Player.h"
#include "Utils/Direction.h"

namespace mob {

    void AggressiveBehaviour::update(Mob* mob)  {
        auto mobPos = mob->getPositionOnMap();
        const Player* player = mob->getWorld()->getNearestPlayer(mobPos);
        auto playerPos = player->getPositionOnMap();
        if (m_attack->isPossibleAttack(mob->getWorld(), mobPos, playerPos - mobPos))
            m_attack->attack(mob->getWorld(), mobPos, Direction(playerPos - mobPos));
        else if (auto pathToPlayer = mob->getWorld()->findPath(mobPos, playerPos);
                      pathToPlayer.size() <= mob->getVisionRange()) {
            mob->setPosition(pathToPlayer.at(0));
        } else {
            // move somewhere (patrol needed)
            DirectionType direction = static_cast<DirectionType>(cocos2d::RandomHelper::random_int(0, 3));
            if (auto possiblePos = mobPos + Direction(direction).getVector();
                     mob->getWorld()->getTileType(possiblePos) == TileType::GROUND) {
                mob->setPosition(possiblePos);
            }
        }
    }
}