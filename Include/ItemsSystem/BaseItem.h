#pragma once

#include "WorldSystem/BaseEntity.h"
#include "WorldSystem/IVisitorEntities.h"

#include "2d/CCSprite.h"
#include "math/CCGeometry.h"


class Player;
class World;

enum ItemTypeSlot
{
    WEAPON = 0,
    ACCESSORY = 1,
    SPELL = 2,
    CONSUMABLE = 3,
};

/**
 * Base item class for every item that can be put in the player's inventory (weapon, consumable, accessory...)
 */
class BaseItem : public BaseEntity
{
public:
    /**
     * Get item image rectangle from tileset
     * @return pixel rectangle
     */
    cocos2d::Rect getRectSpriteInTileset() { return m_rectInTileset; }

    /**
     * Interact with the item
     * @return true if successful, false otherwise
     */
    virtual bool interact() { return false; }

    /**
     * Put item in the player's inventory and remove from the ground
     * @param player player
     */
    virtual void pickUp(Player* player);

    /**
     * Throw item on the ground and remove from player inventory
     */
    virtual void throwOff();

    /**
     * Get item type from slot
     * @return type of the item slot
     */
    virtual ItemTypeSlot getItemTypeFromSlot() const = 0;

    /**
     * @return container with all stats of this item
     */
    const std::shared_ptr<IStatsContainer> getStats() const override = 0;

    /**
     * Accept visitor with given function
     * @param visitor visitor
     */
    void acceptVisit(std::shared_ptr<IVisitorEntities> visitor) override { visitor->visitItem(this); }

protected:
    /**
     * @param world world
     * @param rect pixel rectangle from the tileset
     */
    explicit BaseItem(World* world, const cocos2d::Rect& rect);

    /**
     * @return player owner of this item
     */
    const Player* getPlayer() const { return m_player; }
    
    cocos2d::Rect m_rectInTileset;
    cocos2d::Sprite* m_sprite;
    Player* m_player;
};
