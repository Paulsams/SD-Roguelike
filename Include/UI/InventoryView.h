#pragma once

#include "MenuItemForInventory.h"
#include "ui/UILayout.h"

class InventoryView final : public cocos2d::ui::Layout
{
public:
    enum SelectItemType
    {
        USE,
        DROP,
    };
    
    struct SelectedItemInfo
    {
        SelectedItemInfo(InventoryView* inventory, MenuItemForInventory* item,
            int localIndex, SelectItemType selectType)
            : inventory(inventory)
            , menuItem(item)
            , index(localIndex)
            , selectType(selectType) { }

        InventoryView* inventory;
        MenuItemForInventory* menuItem;
        int index;
        SelectItemType selectType;
    };
    
    using Inventory = ObservableVector<BaseItem*>;
    
    static InventoryView* create(Inventory& observableItems, const std::vector<ItemTypeSlot>& availableSlots,
        const SpriteWithRect& spriteWithRect, cocos2d::Size itemSize, int columns, cocos2d::Vec2 padding);

    bool initWithGrid(cocos2d::Size itemSize, int columns);

    ~InventoryView();

    bool isAvailable(ItemTypeSlot type) { return std::find(m_availableSlots.begin(),
                                                           m_availableSlots.end(), type) != m_availableSlots.end(); }

    void swapItems(int firstIndex, int secondIndex) const { m_observableItems.swap(firstIndex, secondIndex); }
    
    void setItemFromIndex(int index, BaseItem* item) const { m_observableItems.setAt(index, item); }
    
    EventContainer<SelectedItemInfo> selected;

private:
    InventoryView(Inventory& observableItems, const SpriteWithRect& backgroundFrame,
        const std::vector<ItemTypeSlot>& availableSlots, cocos2d::Vec2 padding);

    void onChangeItem(size_t indexChange, Inventory::oldValue, Inventory::newValue newItem) const;

    void onSwappedItems(size_t oldIndex, size_t newIndex) const;

    FunctionHandler<size_t, Inventory::oldValue, Inventory::newValue> m_changeItemDelegate;
    FunctionHandler<size_t, size_t> m_swappedItemsDelegate;

    std::vector<MenuItemForInventory*> m_menuItems;
    SpriteWithRect m_backgroundFrame;
    std::vector<ItemTypeSlot> m_availableSlots;
    Inventory& m_observableItems;
    cocos2d::Vec2 m_padding;
};
