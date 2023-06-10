#include "Mobs/Behaviour/AggressiveBehaviour.h"
#include "Player/Player.h"
#include "Utils/Direction.h"

namespace mob {

    void AggressiveBehaviour::update(Mob* mob)  {
        Vec2Int mobPos = mob->getPositionOnMap();
        const Player* player = mob->getWorld()->getNearestPlayer(mobPos);
        const Vec2Int playerPos = player->getPositionOnMap();
        if (m_attack->isPossibleAttack(mob->getWorld(), mobPos, playerPos - mobPos))
        {
            m_attack->attack(mob->getWorld(), mobPos, Direction(playerPos - mobPos));
            DamageIndicatorsSystems* damageIndicators = mob->getWorld()->getDamageIndicatorsForEnemies();
            m_attack->drawIndicators(mob->getWorld(), damageIndicators, mobPos, Direction(RIGHT));
            m_attack->drawIndicators(mob->getWorld(), damageIndicators, mobPos, Direction(UP));
            m_attack->drawIndicators(mob->getWorld(), damageIndicators, mobPos, Direction(LEFT));
            m_attack->drawIndicators(mob->getWorld(), damageIndicators, mobPos, Direction(DOWN));
        }
        else if (auto pathToPlayer = mob->getWorld()->findPath(mobPos, playerPos);
                      pathToPlayer.size() <= mob->getVisionRange())
        {
            mobPos = pathToPlayer.at(1);
            mob->setMovedPositionOnMap(mobPos);
            DamageIndicatorsSystems* damageIndicators = mob->getWorld()->getDamageIndicatorsForEnemies();
            m_attack->drawIndicators(mob->getWorld(), damageIndicators, mobPos, Direction(RIGHT));
            m_attack->drawIndicators(mob->getWorld(), damageIndicators, mobPos, Direction(UP));
            m_attack->drawIndicators(mob->getWorld(), damageIndicators, mobPos, Direction(LEFT));
            m_attack->drawIndicators(mob->getWorld(), damageIndicators, mobPos, Direction(DOWN));
        }
        else
        {
            // move somewhere (patrol needed)
            DirectionType direction = static_cast<DirectionType>(cocos2d::RandomHelper::random_int(0, 3));
            const Vec2Int possiblePos = mobPos + Direction(direction).getVector();
            if (mob->getWorld()->getTileType(possiblePos) == TileType::GROUND)
            {
                mob->setMovedPositionOnMap(possiblePos);
            }
        }
    }
}