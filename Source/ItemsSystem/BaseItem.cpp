#include "ItemsSystem/BaseItem.h"

#include "Player/Player.h"
#include "WorldSystem/World.h"

void BaseItem::pickUp(Player* player)
{
    m_player = player;
    m_sprite->setVisible(false);
    retain();
    getWorld()->removeEntity(this);
}

void BaseItem::throwOff()
{
    m_sprite->setVisible(true);
    setPositionOnMapWithoutNotify(m_player->getPositionOnMap());
    getWorld()->addEntity(this);
    release();
    
    m_player = nullptr;
}

BaseItem::BaseItem(World* world, const cocos2d::Rect& rect)
    : BaseEntity(world)
    , m_rectInTileset(rect)
    , m_player(nullptr)
{
    m_sprite = cocos2d::Sprite::create(Paths::toGameTileset, rect);
    m_sprite->setAnchorPoint(cocos2d::Vec2::ZERO);
    Node::addChild(m_sprite);
}
