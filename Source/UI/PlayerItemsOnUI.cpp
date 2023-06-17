#include "UI/PlayerItemsOnUI.h"

PlayerItemsOnUI::PlayerItemsOnUI(InventoryView* inventory, InventoryView* weapons,
    InventoryView* accessories, InventoryView* spells)
    : m_selectedDelegate(CC_CALLBACK_1(PlayerItemsOnUI::onSelected, this))
    , m_inventory(inventory)
    , m_weapons(weapons)
    , m_accessories(accessories)
    , m_spells(spells)
{
    m_inventory->selected += m_selectedDelegate;
    m_weapons->selected += m_selectedDelegate;
    m_accessories->selected += m_selectedDelegate;
    m_spells->selected += m_selectedDelegate;
}

PlayerItemsOnUI::~PlayerItemsOnUI()
{
    m_inventory->selected -= m_selectedDelegate;
    m_weapons->selected -= m_selectedDelegate;
    m_accessories->selected -= m_selectedDelegate;
    m_spells->selected -= m_selectedDelegate;
}

void PlayerItemsOnUI::onSelected(InventoryView::SelectedItemInfo selectedInfo)
{
    if (!m_selectedInfo.has_value())
    {
        BaseItem* item = selectedInfo.menuItem->getItem();
        
        if (item == nullptr)
            return;

        if (selectedInfo.selectType == InventoryView::USE)
        {
            if (item->interact())
            {
                selectedInfo.inventory->setItemFromIndex(selectedInfo.index, nullptr);
                selectedInfo.menuItem->unchoice();
                item->release();
                return;
            }
        
            m_selectedInfo = selectedInfo;
            m_selectedInfo->menuItem->choice();
        }
        else
        {
            selectedInfo.inventory->setItemFromIndex(selectedInfo.index, nullptr);
            selectedInfo.menuItem->unchoice();
            item->throwOff();
        }
        
        return; 
    }

    if (m_selectedInfo->inventory == selectedInfo.inventory)
    {
        selectedInfo.inventory->swapItems(m_selectedInfo->index, selectedInfo.index);
    }
    else if (selectedInfo.inventory->isAvailable(m_selectedInfo->menuItem->getItem()->getItemTypeFromSlot()))
    {
        BaseItem* oldItem = selectedInfo.menuItem->getItem();
        selectedInfo.inventory->setItemFromIndex(selectedInfo.index, m_selectedInfo->menuItem->getItem());
        m_selectedInfo->inventory->setItemFromIndex(m_selectedInfo->index, oldItem);
    }

    m_selectedInfo->menuItem->unchoice();
    m_selectedInfo.reset();
}
