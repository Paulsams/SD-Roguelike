#include "UI/InventoryView.h"

InventoryView* InventoryView::create(Inventory& observableItems, const std::vector<ItemTypeSlot>& availableSlots,
    const SpriteWithRect& spriteWithRect, cocos2d::Size itemSize, int columns, cocos2d::Vec2 padding)
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

bool InventoryView::initWithGrid(cocos2d::Size itemSize, int columns)
{
    const std::vector<BaseItem*> items = m_observableItems.getCollection();
    for (size_t i = 0; i < items.size(); ++i)
    {
        MenuItemForInventory* menuItem = MenuItemForInventory::create(itemSize, m_backgroundFrame,
            [this, i](Ref* ref)
            {
              MenuItemForInventory* menuItem = dynamic_cast<MenuItemForInventory*>(ref);
              this->selected.invoke(SelectedItemInfo(this, menuItem, i));
            });
        menuItem->setItem(items[i]);

        m_menuItems.push_back(menuItem);
    }

    cocos2d::Menu* menu = cocos2d::Menu::create();
    if (!menu)
        return false;
        
    for (const auto& item : m_menuItems)
    {
        menu->addChild(item);
    }
        
    int count = menu->getChildrenCount();
    int numRows = (count + columns - 1) / columns;
    int numCols = std::min(count, columns);
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

InventoryView::~InventoryView()
{
    // m_observableItems.changed -= m_changeItemDelegate;
    // m_observableItems.swapped -= m_swappedItemsDelegate;
}

InventoryView::InventoryView(Inventory& observableItems, const SpriteWithRect& backgroundFrame,
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

void InventoryView::onChangeItem(size_t indexChange, Inventory::oldValue, Inventory::newValue newItem) const
{
    m_menuItems[indexChange]->setItem(newItem);
}

void InventoryView::onSwappedItems(size_t oldIndex, size_t newIndex) const
{
    BaseItem* oldItem = m_menuItems[oldIndex]->getItem();
    m_menuItems[oldIndex]->setItem(m_menuItems[newIndex]->getItem());
    m_menuItems[newIndex]->setItem(oldItem);
}
