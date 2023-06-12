#include "Mobs/Behaviour/CowardlyBehaviour.h"
#include "Player/Player.h"

namespace mob {

    void CowardlyBehaviour::update(Mob* mob) {
        const auto mobPos = mob->getPositionOnMap();
        const Player* player = mob->getWorld()->getNearestPlayer(mobPos);
        const auto playerPos = player->getPositionOnMap();
        if (m_attack->isPossibleAttack(mob->getWorld(), mobPos, playerPos - mobPos)) {
            m_attack->attack(mob->getWorld(), mobPos, Direction(playerPos - mobPos));
        } else if (const auto distance = playerPos.distance(mobPos); distance <= mob->getVisionRange()) {
            auto delta =  playerPos - mobPos;
            const auto direction = Direction(playerPos - mobPos);
            // Vec2Int offset;
            // if (std::abs(delta.x) < std::abs(delta.y)) {
            //     offset = delta.x < 0 ? Vec2Int(1, 0) : Vec2Int(-1, 0);
            // } else {
            //     offset = delta.y < 0 ? Vec2Int(0, 1) : Vec2Int(0, -1);
            // }

            if (const auto possiblePos = mobPos - direction.getVector();
                    mob->getWorld()->getTileType(possiblePos) == TileType::GROUND) {
                mob->setScheduleMovePositionOnMap(possiblePos);
            }
        }
    }
}