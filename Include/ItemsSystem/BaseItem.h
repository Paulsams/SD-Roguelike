#pragma once

#include "2d/CCSprite.h"
#include "math/CCGeometry.h"
#include "Utils/Common.h"
#include "WorldSystem/BaseEntity.h"
#include "Stats/IStatsContainer.h"
#include "WorldSystem/IVisitorEntities.h"
#include "WorldSystem/World.h"

class Player;

enum ItemTypeSlot
{
    WEAPON = 0,
    ACCESSORY = 1,
    SPELL = 2,
};

class BaseItem : public BaseEntity
{
public:
    // virtual std::string getName() const = 0;
    // virtual std::string getDescription() const = 0;
    cocos2d::Rect getRectSpriteInTileset() { return m_rectInTileset; }
    virtual bool interact() { return false; }

    virtual void pickUp(Player* player)
    {
        m_player = player;
        m_sprite->setVisible(false);
        getWorld()->removeEntity(this);
    }
    
    virtual void throwOff()
    {
        m_player = nullptr;
        m_sprite->setVisible(true);
        getWorld()->addEntity(this);
    }

    virtual ItemTypeSlot getItemTypeFromSlot() const = 0;
    const std::shared_ptr<IStatsContainer> getStats() const override = 0;

    void acceptVisit(std::shared_ptr<IVisitorEntities> visitor) override { visitor->visitItem(this); }

protected:
    explicit BaseItem(World* world, const cocos2d::Rect& rect)
        : BaseEntity(world)
        , m_rectInTileset(rect)
        , m_player(nullptr)
    {
        m_sprite = cocos2d::Sprite::create(Paths::toGameTileset, rect);
        m_sprite->setAnchorPoint(cocos2d::Vec2::ZERO);
        Node::addChild(m_sprite);
    }

    const Player* getPlayer() const { return m_player; }

private:
    cocos2d::Rect m_rectInTileset;
    cocos2d::Sprite* m_sprite;
    Player* m_player;
};
