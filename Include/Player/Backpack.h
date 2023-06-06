#pragma once
#include <vector>

#include "ItemsSystem/Accessory.h"
#include "ItemsSystem/IDealingDamage.h"
#include "ItemsSystem/Weapon.h"

class Backpack : public IDealingDamage
{
    
    
private:
    Weapon weapon;
    std::vector<Accessory> accessory;
};
