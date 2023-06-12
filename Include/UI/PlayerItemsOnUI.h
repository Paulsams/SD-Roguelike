#pragma once
#include <optional>

#include "UI/InventoryView.h"

class PlayerItemsOnUI
{
public:
    PlayerItemsOnUI(InventoryView* inventory, InventoryView* weapons,
        InventoryView* accessories, InventoryView* spells);

private:
    FunctionHandler<InventoryView::SelectedItemInfo> m_selectedDelegate;
    
    void OnSelected(InventoryView::SelectedItemInfo selectedInfo);

    InventoryView* m_inventory;
    InventoryView* m_weapons;
    InventoryView* m_accessories;
    InventoryView* m_spells;

    std::optional<InventoryView::SelectedItemInfo> m_selectedInfo;
};
