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
            drawDamageIndicators(mob);
        }
        else if (playerPos.distance(mobPos) <= mob->getVisionRange())
        {
            if (auto pathToPlayer = mob->getWorld()->findPath(mobPos, playerPos);
                pathToPlayer.size() <= mob->getVisionRange() && !pathToPlayer.empty())
            {
                mobPos = pathToPlayer.at(0);
                mob->setScheduleMovePositionOnMap(mobPos);
                drawDamageIndicators(mob);
            }
        }
        else
        {
            // move somewhere (patrol needed)
            DirectionType direction = static_cast<DirectionType>(cocos2d::RandomHelper::random_int(0, 3));
            const Vec2Int possiblePos = mobPos + Direction(direction).getVector();
            if (mob->getWorld()->getTileType(possiblePos) == TileType::GROUND)
            {
                mob->setScheduleMovePositionOnMap(possiblePos);
            }
        }
    }

    void AggressiveBehaviour::drawDamageIndicators(Mob* mob) const
    {
        DamageIndicatorsSystems* damageIndicators = mob->getWorld()->getDamageIndicatorsForMobs();
        damageIndicators->scheduleDraw([this, mob](const std::function<void(DrawDamageInfo)>& drawFunc)
        {
            m_attack->drawIndicators(mob->getWorld(), mob->getPositionOnMap(), Direction(RIGHT), drawFunc);
            m_attack->drawIndicators(mob->getWorld(), mob->getPositionOnMap(), Direction(UP), drawFunc);
            m_attack->drawIndicators(mob->getWorld(), mob->getPositionOnMap(), Direction(LEFT), drawFunc);
            m_attack->drawIndicators(mob->getWorld(), mob->getPositionOnMap(), Direction(DOWN), drawFunc);
        });
    }
}