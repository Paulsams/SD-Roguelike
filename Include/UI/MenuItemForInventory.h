#pragma once

#include "2d/CCMenuItem.h"
#include "ItemsSystem/BaseItem.h"
#include "ui/UIImageView.h"
#include "Utils/Common.h"

class MenuItemForInventory final : public cocos2d::MenuItem
{
public:
    static MenuItemForInventory* create(cocos2d::Size size,
        const SpriteWithRect& sprite, const cocos2d::ccMenuCallback& callback);
    
    void setItem(BaseItem* item);

    void choice();
    void unchoice();

    bool initWithSprite(const SpriteWithRect& sprite);
    BaseItem* getItem() const { return m_item; }

private:
    MenuItemForInventory() = default;

    cocos2d::ui::ImageView* m_frame = nullptr;
    cocos2d::ui::ImageView* m_backgroundItemIcon = nullptr;
    cocos2d::ui::ImageView* m_itemIcon = nullptr;
    BaseItem* m_item;
};
