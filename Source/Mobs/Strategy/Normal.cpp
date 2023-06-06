#include "Mobs/Strategy/Normal.h"


namespace mob {
    void Normal::update(Mob *mob) {
        mob->changeBehaviour(aggressiveBehaviour);
    }
}