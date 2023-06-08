#include "Mobs/Behaviour/AggressiveBehaviour.h"

namespace mob {

    void AggressiveBehaviour::update(Mob* mob)  {
        auto playerPos = mob->getWorld()->getNearestPlayer(mob->getPositionOnMap());
        
    }
}