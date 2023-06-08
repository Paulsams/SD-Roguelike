#pragma once
#include <optional>

#include "ItemsSystem/Accessory.h"
#include "ItemsSystem/Weapon.h"
#include "UI/InventoryView.h"

class PlayerItemsOnUI
{
public:
    PlayerItemsOnUI(InventoryView<BaseItem>* inventory, InventoryView<Weapon>* weapons,
        InventoryView<Accessory>* accessories, InventoryView<Weapon>* spells)
        : m_inventory(inventory)
        , m_weapons(weapons)
        , m_accessories(accessories)
        , m_spells(spells) { }

private:
    void OnSelected(InventoryView<BaseItem>::SelectedItemInfo selectedInfo)
    {
        if (!m_selectedInfo.has_value())
        {
            m_selectedInfo = selectedInfo;
            return;
        }

        if (m_selectedInfo->inventory == selectedInfo.inventory)
        {
            selectedInfo.inventory->swapItems(m_selectedInfo->index, selectedInfo.index);
        }
        else if (selectedInfo.inventory->isAvailable(m_selectedInfo->menuItem->getItem()->getItemTypeFromSlot()))
        {
            selectedInfo.inventory->setItemFromIndex(selectedInfo.index, m_selectedInfo->menuItem->getItem());
            m_selectedInfo->inventory->setItemFromIndex(m_selectedInfo->index, selectedInfo.menuItem->getItem());
        }

        m_selectedInfo.reset();
    }
    
    InventoryView<BaseItem>* m_inventory;
    InventoryView<Weapon>* m_weapons;
    InventoryView<Accessory>* m_accessories;
    InventoryView<Weapon>* m_spells;

    std::optional<InventoryView<BaseItem>::SelectedItemInfo> m_selectedInfo;
};
