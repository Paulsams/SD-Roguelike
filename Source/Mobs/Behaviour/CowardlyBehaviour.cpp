#include "Mobs/Behaviour/CowardlyBehaviour.h"
#include "Player/Player.h"

namespace mob
{

void CowardlyBehaviour::update(Mob* mob)
{
    const auto mobPos = mob->getPositionOnMap();
    const Player* player = mob->getWorld()->getNearestPlayer(mobPos);

    if (player)
    {
        const auto playerPos = player->getPositionOnMap();
        //      if (m_attack->isPossibleAttack(mob->getWorld(), mobPos, playerPos - mobPos)) {
        //            m_attack->attack(mob->getWorld(), mobPos, Direction(playerPos - mobPos));
        //        } else
        if (const auto distance = playerPos.distance(mobPos); distance <= mob->getVisionRange())
        {
            auto delta =  playerPos - mobPos;
            const auto direction = Direction(playerPos - mobPos);

            if (const auto possiblePos = mobPos - direction.getVector();
                    mob->getWorld()->getTileType(possiblePos) == TileType::GROUND)
            {
                mob->setScheduleMovePositionOnMap(possiblePos);
            }
        }

        return;
    }

    const DirectionType direction = static_cast<DirectionType>(cocos2d::RandomHelper::random_int(0, 3));
    const Vec2Int possiblePos = mobPos + Direction(direction).getVector();
    if (mob->getWorld()->getTileType(possiblePos) == TileType::GROUND)
        mob->setScheduleMovePositionOnMap(possiblePos);
}

}