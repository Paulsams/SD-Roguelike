#pragma once

#include "2d/CCMenu.h"
#include "ItemsSystem/IItem.h"
#include "ui/UILayout.h"
#include "Utils/EventsSystem.h"

class InventoryView final : public cocos2d::ui::Layout
{
public:
    using Inventory = ObservableVector<std::shared_ptr<IItem>>;
    
    static InventoryView* create(Inventory& observableItems, cocos2d::Size contentSize, cocos2d::Vec2 padding);

    bool initWithContentSize(cocos2d::Size contentSize);

    ~InventoryView() override;

private:
    InventoryView(Inventory& observableItems, cocos2d::Vec2 padding);

    void onChangeItem(size_t indexChange, Inventory::oldValue oldItem, Inventory::newValue newItem);
    void onSwappedItems(size_t oldIndex, size_t newIndex);

    FunctionHandler<size_t, Inventory::oldValue, Inventory::newValue> m_changeItemDelegate;
    FunctionHandler<size_t, size_t> m_swappedItemsDelegate;
    
    Inventory& m_observableItems;
    cocos2d::Vec2 m_padding;
};
