#include "UI/InventoryView.h"

#include "2d/CCMenuItem.h"
#include "ui/UIButton.h"
#include "ui/UIImageView.h"
#include "Utils/Common.h"

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
    static MenuItemForInventory* create(Size size, const ccMenuCallback& callback)
    {
        MenuItemForInventory* menuItem = new (std::nothrow) MenuItemForInventory();
        menuItem->setContentSize(size);
        
        if (menuItem && menuItem->init() && menuItem->initWithCallback(callback))
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
            m_itemIcon->setTextureRect(item->getRectSpriteInTileset());
            addChild(m_itemIcon);
        }
        m_itemIcon->setEnabled(static_cast<bool>(item));
    }
    
    void selected() override
    {
        MenuItem::selected();
        setColor(Color3B::GRAY);
    }

    void unselected() override
    {
        MenuItem::unselected();
        setColor(Color3B::WHITE);
    }

    bool init() override
    {
        m_frame = ImageView::create(Paths::toIconFrame);
        m_frame->ignoreContentAdaptWithSize(false);
        m_frame->setContentSize(getContentSize());
        m_frame->setAnchorPoint(Vec2::ZERO);
        this->addChild(m_frame);

        m_itemIcon = ImageView::create(Paths::toUITileset);
        m_itemIcon->setEnabled(false);
        m_itemIcon->ignoreContentAdaptWithSize(false);
        m_itemIcon->setAnchorPoint(Vec2::ZERO);
        this->addChild(m_itemIcon);
        
        return true;
    }
    
private:
    MenuItemForInventory() = default;
    
    ImageView* m_frame;
    ImageView* m_itemIcon;
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
        MenuItemForInventory* menuItem = MenuItemForInventory::create(itemSize, nullptr);
        menuItem->setItem(item);

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
