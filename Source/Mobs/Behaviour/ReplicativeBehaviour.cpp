#include "Mobs/Behaviour/ReplicativeBehaviour.h"
#include "Mobs/Behaviour/AggressiveBehaviour.h"
#include "Player/Player.h"

namespace mob
{

    void ReplicativeBehaviour::update(Mob* mob)
    {
        static std::shared_ptr<FunctionVisitorEntities<bool>> visitor = FunctionVisitorEntitiesBuilder<bool>()
                .setMob([](mob::Mob*)          { return false; })
                .setPlayer([](Player*)         { return false; })
                .build();

        const auto mobPos = mob->getPositionOnMap();
        const Player* player = mob->getWorld()->getNearestPlayer(mobPos);

        if (player)
        {
            const auto playerPos = player->getPositionOnMap();
            if (m_attack->isPossibleAttack(mob->getWorld(), mobPos, playerPos - mobPos)) {
                m_attack->attack(mob->getWorld(), mobPos, Direction(playerPos - mobPos));
            } else if (const auto distance = playerPos.distance(mobPos); distance <= mob->getVisionRange())
            {
                auto delta =  playerPos - mobPos;
                const auto direction = Direction(playerPos - mobPos);

                if (const auto possiblePos = mobPos - direction.getVector(); mob->getWorld()->getTileType(possiblePos) == TileType::GROUND)
                {
                    mob->setScheduleMovePositionOnMap(possiblePos);
                }
            }

            if (cocos2d::random(0.0, 1.0) < 0.25)
            {
                Vec2Int pos = mob->getPositionOnMap();
                for (int i = -1; i <= 1; ++i)
                {
                    for (int j = -1; j <= 1; ++j)
                    {
                        if (i == 0 && j == 0)
                            continue;

                        Vec2Int newPos = pos + Vec2Int{i, j};

                        if (mob->getWorld()->getTileType(newPos) == TileType::GROUND)
                        {
                            bool isSpawn = true;
                            for (BaseEntity* entity : mob->getWorld()->getEntitiesFromCell(newPos))
                            {
                                mob->acceptVisit(visitor);
                                isSpawn = visitor->getReturnValue().value_or(true);
                                if (!isSpawn)
                                    break;
                            }
                            if (!isSpawn)
                                continue;
                            Mob* clone = mob->clone();
                            clone->setPositionOnMapWithoutNotify(pos + Vec2Int{i, j});
                            mob->getWorld()->addEntity(clone);
                            mob->changeBehaviour(std::make_shared<AggressiveBehaviour>(m_attack));
                            return;
                        }
                    }
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