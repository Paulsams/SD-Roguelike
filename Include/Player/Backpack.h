#pragma once

#include "ItemsSystem/Accessory.h"
#include "ItemsSystem/Weapon.h"

class Backpack
{
public:
    Backpack()
    {
        m_weapon.push_back(nullptr);

        for (int i = 0; i < 3; ++i) 
            m_accessories.push_back(nullptr);

        for (int i = 0; i < 4; ++i) 
            m_spells.push_back(nullptr);
    }

    ObservableVector<Weapon*>& getWeapons() { return m_weapon; }
    ObservableVector<Accessory*>& getAccessories() { return m_accessories; }
    ObservableVector<Weapon*>& getSpells() { return m_spells; }
    
private:
    ObservableVector<Weapon*> m_weapon;
    ObservableVector<Accessory*> m_accessories;
    ObservableVector<Weapon*> m_spells;
};
