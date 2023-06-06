#pragma once

#include <ItemsSystem/IDealingDamage.h>
#include "ItemsSystem/Accessory.h"
#include "ItemsSystem/Weapon.h"

class Backpack // : public IDealingDamage
{
// public:
//     float getDamage(const BaseEntity* otherEntity) const override;
//     
private:
    Weapon weapon;
    std::vector<Accessory> accessory;
};
