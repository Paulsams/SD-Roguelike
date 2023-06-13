#pragma once

#include "2d/CCSprite.h"
#include "math/CCGeometry.h"
#include "WorldSystem/BaseEntity.h"
#include "WorldSystem/IVisitorEntities.h"

class Player;
class World;

enum ItemTypeSlot
{
    WEAPON = 0,
    ACCESSORY = 1,
    SPELL = 2,
    CONSUMABLE = 3,
};

class BaseItem : public BaseEntity
{
public:
    // virtual std::string getName() const = 0;
    // virtual std::string getDescription() const = 0;
    cocos2d::Rect getRectSpriteInTileset() { return m_rectInTileset; }
    virtual bool interact() { return false; }

    virtual void pickUp(Player* player);
    
    virtual void throwOff();

    virtual ItemTypeSlot getItemTypeFromSlot() const = 0;
    const std::shared_ptr<IStatsContainer> getStats() const override = 0;

    void acceptVisit(std::shared_ptr<IVisitorEntities> visitor) override { visitor->visitItem(this); }

protected:
    explicit BaseItem(World* world, const cocos2d::Rect& rect);
    
    const Player* getPlayer() const { return m_player; }
private:
    cocos2d::Rect m_rectInTileset;
    cocos2d::Sprite* m_sprite;
    Player* m_player;
};
