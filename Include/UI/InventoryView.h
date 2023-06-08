#pragma once

#include "Utils/Common.h"
#include "MenuItemForInventory.h"
#include "2d/CCMenu.h"
#include "ItemsSystem/BaseItem.h"
#include "ui/UILayout.h"
#include "Utils/EventsSystem.h"

template<Derived<BaseItem> T>
class InventoryView final : public cocos2d::ui::Layout
{
public:
    struct SelectedItemInfo
    {
        SelectedItemInfo(InventoryView* inventory, MenuItemForInventory* item, int localIndex)
            : inventory(inventory),
              menuItem(item),
              index(localIndex) { }

        InventoryView* inventory;
        MenuItemForInventory* menuItem;
        int index;
    };
    
    using Inventory = ObservableVector<T*>;
    
    static InventoryView* create(Inventory& observableItems, const std::vector<ItemTypeSlot>& availableSlots,
        const SpriteWithRect& spriteWithRect, cocos2d::Size itemSize, uint32_t columns, cocos2d::Vec2 padding)
    {
        auto inventoryView = new (std::nothrow) InventoryView(observableItems, spriteWithRect, availableSlots, padding);
        if (inventoryView && inventoryView->initWithGrid(itemSize, columns))
        {
            inventoryView->autorelease();
            return inventoryView;
        }
        CC_SAFE_DELETE(inventoryView);
        return nullptr;
    }

    bool initWithGrid(cocos2d::Size itemSize, uint32_t columns)
    {
        for (T* const item : m_observableItems.getCollection())
        {
            MenuItemForInventory* menuItem = MenuItemForInventory::create(itemSize, m_backgroundFrame, nullptr);
            menuItem->setItem(item);

            m_menuItems.push_back(menuItem);
        }

        cocos2d::Menu* menu = cocos2d::Menu::create();
        if (!menu)
            return false;
        
        for (const auto& item : m_menuItems)
        {
            menu->addChild(item);
        }
    
        // set content size
        int count = menu->getChildrenCount();
        int numRows = (count + columns - 1) / columns;
        int numCols = MIN(count, columns);
        float height = itemSize.height * numRows + m_padding.y * (numRows - 1);
        float width = itemSize.width * numCols + m_padding.x * (numCols - 1);
        this->setContentSize({width, height});

        int row = 0;
        int col = 0;
        for(auto item : menu->getChildren()) {
            float x = (itemSize.width + m_padding.x) * col + itemSize.width * 0.5f;
            float y = height - (itemSize.height + m_padding.y) * row - itemSize.height * 0.5f;
            item->setPosition(x, y);
        
            if(++col == columns) {
                col = 0;
                row++;
            }
        }

        menu->setPosition(cocos2d::Vec2::ZERO);
        this->addChild(menu);
    
        return true;
    }
    
    ~InventoryView() override
    {
        // m_observableItems.changed -= m_changeItemDelegate;
        // m_observableItems.swapped -= m_swappedItemsDelegate;
    }

    bool isAvailable(ItemTypeSlot type) { return std::find(m_availableSlots.begin(),
        m_availableSlots.end(), type) != m_availableSlots.end(); }

    void swapItems(int firstIndex, int secondIndex)
    {
        
    }
    
    void setItemFromIndex(int index, BaseItem* item)
    {
        
    }

private:
    InventoryView(Inventory& observableItems, const SpriteWithRect& backgroundFrame,
        const std::vector<ItemTypeSlot>& availableSlots, cocos2d::Vec2 padding)
    : m_changeItemDelegate(CC_CALLBACK_3(InventoryView::onChangeItem, this))
    , m_swappedItemsDelegate(CC_CALLBACK_2(InventoryView::onSwappedItems, this))
    , m_backgroundFrame(backgroundFrame)
    , m_availableSlots(availableSlots)
    , m_observableItems(observableItems)
    , m_padding(padding)
    {
        m_observableItems.changed += m_changeItemDelegate;
        m_observableItems.swapped += m_swappedItemsDelegate;
    }

    void onChangeItem(size_t indexChange, typename Inventory::oldValue oldItem, typename Inventory::newValue newItem)
    {
        m_menuItems[indexChange]->setItem(newItem);
    }

    void onSwappedItems(size_t oldIndex, size_t newIndex)
    {
        
    }

    FunctionHandler<size_t, typename Inventory::oldValue, typename Inventory::newValue> m_changeItemDelegate;
    FunctionHandler<size_t, size_t> m_swappedItemsDelegate;

    std::vector<MenuItemForInventory*> m_menuItems;
    SpriteWithRect m_backgroundFrame;
    std::vector<ItemTypeSlot> m_availableSlots;
    Inventory& m_observableItems;
    cocos2d::Vec2 m_padding;
};
