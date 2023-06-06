#include "UI/InventoryView.h"

#include "2d/CCMenuItem.h"
#include "ui/UIButton.h"

using namespace cocos2d;
using namespace cocos2d::ui;

InventoryView* InventoryView::create(Inventory& observableItems, Size contentSize, Vec2 padding)
{
    InventoryView* inventoryView = new (std::nothrow) InventoryView(observableItems, padding);
    if (inventoryView && inventoryView->initWithContentSize(contentSize))
    {
        inventoryView->autorelease();
        return inventoryView;
    }
    CC_SAFE_DELETE(inventoryView);
    return nullptr;
}

class MenuItemForInventory : public MenuItem
{
public:
    static MenuItemForInventory* create(const ccMenuCallback& callback)
    {
        MenuItemForInventory* menuItem = new (std::nothrow) MenuItemForInventory();
        if (menuItem && menuItem->initWithCallback(callback))
        {
            menuItem->autorelease();
            return menuItem;
        }
        CC_SAFE_DELETE(menuItem);
        return nullptr;
    }

    void setItem(std::shared_ptr<IItem> item)
    {
        if (m_itemIcon)
            removeChild(m_itemIcon, true);
        
        if (item)
        {
            m_itemIcon = item->createNewSprite();
            addChild(m_itemIcon);
        }
        
    }
    
    void selected() override
    {
        MenuItem::selected();
    }

    bool init() override
    {
        Sprite* frame = Sprite::create("IconFrame.png");
        frame->setAnchorPoint(Vec2::ZERO);
        frame->addChild(frame);
        
        return true;
    }
    
private:
    MenuItemForInventory() = default;

    Sprite* m_itemIcon;
};

bool InventoryView::initWithContentSize(Size contentSize)
{
    this->setContentSize(contentSize);

    Size itemSize = Size(90.0f, 90.0f);
    // int columns = contentSize.height / itemSize.height;
    int columns = 3;
    
    Vector<MenuItem*> menuItems;

    for (const std::shared_ptr<IItem>& item : m_observableItems.getCollection())
    {
        Sprite* sprite = Sprite::create("Player.png");
        if (item)
            sprite = item->createNewSprite();
        MenuItemForInventory* menuItem = MenuItemForInventory::create(
            [sprite](Ref* ref) { sprite->setColor(Color3B::RED); });
        menuItem->setContentSize(itemSize);
        sprite->setAnchorPoint(Vec2::ZERO);
        sprite->setContentSize(itemSize);
        //menuItem->setEnabled(false);
        menuItem->addChild(sprite);

        menuItems.pushBack(menuItem);
    }

    Menu* menu = Menu::createWithArray(menuItems);
    if (!menu)
        return false;
    
    // set content size
    int count = menu->getChildrenCount();
    int numRows = (count + columns - 1) / columns;
    int numCols = std::min(count, columns);
    float height = itemSize.height * numRows + m_padding.y * (numRows - 1);
    float width = itemSize.width * numCols + m_padding.x * (numCols - 1);

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

    menu->setPosition(Vec2::ZERO);
    this->addChild(menu);
    
    return true;
}

InventoryView::~InventoryView()
{
    // m_observableItems.changed -= m_changeItemDelegate;
    // m_observableItems.swapped -= m_swappedItemsDelegate;
}

InventoryView::InventoryView(Inventory& observableItems, Vec2 padding)
    : m_changeItemDelegate(CC_CALLBACK_3(InventoryView::onChangeItem, this))
    , m_swappedItemsDelegate(CC_CALLBACK_2(InventoryView::onSwappedItems, this))
    , m_observableItems(observableItems)
    , m_padding(padding)
{
    m_observableItems.changed += m_changeItemDelegate;
    m_observableItems.swapped += m_swappedItemsDelegate;
}

void InventoryView::onChangeItem(size_t indexChange, Inventory::oldValue oldItem, Inventory::newValue newItem)
{
    
}

void InventoryView::onSwappedItems(size_t oldIndex, size_t newIndex)
{
    
}
