#include "Mobs/Behaviour/AggressiveBehaviour.h"
#include "Player/Player.h"
#include "Utils/Direction.h"

namespace mob {

    void AggressiveBehaviour::update(Mob* mob)  {
        const Player* player = mob->getWorld()->getNearestPlayer(mob->getPositionInWorld());
        auto pathToPlayer = mob->getWorld()->searchPath(mob->getPositionInWorld(), player->getPositionInWorld());
        if (m_attack->isPossibleAttack(mob->getWorld(), mob->getPositionInWorld(), player->getPositionInWorld() - mob->getPositionInWorld()))
            m_attack->attack(mob->getWorld(), mob->getPositionInWorld());
        else if (pathToPlayer.size() <= mob->getVisionRange()) {
            mob->setPosition(pathToPlayer.at(0));
        } else {
            // move somewhere (patrol neede)
            DirectionType direction = static_cast<DirectionType>(cocos2d::RandomHelper::random_int(0, 3));
            auto possiblePos = mob->getPositionInWorld() + Direction(direction).getVector();
            if (mob->getWorld()->getTileType(possiblePos) == TileType::GROUND)
                mob->setPosition(possiblePos);
        }
    }
}