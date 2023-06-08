#include "UI/MenuItemForInventory.h"

using namespace cocos2d;
using namespace cocos2d::ui;

MenuItemForInventory* MenuItemForInventory::create(Size size, const SpriteWithRect& sprite, const ccMenuCallback& callback)
{
    MenuItemForInventory* menuItem = new (std::nothrow) MenuItemForInventory();
    menuItem->setContentSize(size);
        
    if (menuItem && menuItem->initWithCallback(callback) && menuItem->initWithSprite(sprite))
    {
        menuItem->autorelease();
        return menuItem;
    }
    CC_SAFE_DELETE(menuItem);
    return nullptr;
}

void MenuItemForInventory::setItem(BaseItem* item)
{
    m_item = item;
    const bool isValidItem = static_cast<bool>(item);
    if (isValidItem)
        m_itemIcon->setTextureRect(item->getRectSpriteInTileset());
    m_itemIcon->setVisible(isValidItem);
    m_backgroundItemIcon->setOpacity(isValidItem ? 50 : 128);
}

void MenuItemForInventory::choice()
{
    setColor(Color3B::GRAY);
    m_frame->setColor(Color3B::GRAY);
}

void MenuItemForInventory::unchoice()
{
    setColor(Color3B::WHITE);
    m_frame->setColor(Color3B::WHITE);
}

void MenuItemForInventory::selected()
{
    MenuItem::selected();
    
    if (!m_item)
        return;

    if (m_item->interact())
    {
        setItem(nullptr);
        unchoice();
    }
}

bool MenuItemForInventory::initWithSprite(const SpriteWithRect& sprite)
{
    m_frame = ImageView::create(Paths::toIconFrame);
    m_frame->ignoreContentAdaptWithSize(false);
    m_frame->setContentSize(getContentSize());
    m_frame->setAnchorPoint(Vec2::ZERO);
    this->addChild(m_frame);

    m_backgroundItemIcon = ImageView::create(sprite.first);
    m_backgroundItemIcon->setTextureRect(sprite.second);
    m_backgroundItemIcon->ignoreContentAdaptWithSize(false);
    m_backgroundItemIcon->setOpacity(128);
    m_backgroundItemIcon->setColor(Color3B::GRAY);
    m_backgroundItemIcon->setContentSize(getContentSize() * 0.7f);
    m_backgroundItemIcon->setPosition(m_backgroundItemIcon->getContentSize() / 4.0f);
    m_backgroundItemIcon->setAnchorPoint(Vec2::ZERO);
    this->addChild(m_backgroundItemIcon);

    m_itemIcon = ImageView::create(Paths::toUITileset);
    m_itemIcon->setVisible(false);
    m_itemIcon->setContentSize(getContentSize());
    m_itemIcon->ignoreContentAdaptWithSize(false);
    m_itemIcon->setAnchorPoint(Vec2::ZERO);
    this->addChild(m_itemIcon);
        
    return true;
}
