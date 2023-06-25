#pragma once

class Player;
class Chest;
class BaseItem;
class Decoration;

namespace mob
{
    class Mob;
}

struct IVisitorEntities
{
    virtual ~IVisitorEntities() = default;

    /**
     * Implementing a visit for a mob
     * @param mob mob
     */
    virtual void visitMob(mob::Mob* mob) = 0;
    /**
     * Implementing a visit for a player
     * @param player player
     */
    virtual void visitPlayer(Player* player) = 0;
    /**
     * Implementing a visit for a item
     * @param item item
     */
    virtual void visitItem(BaseItem* item) = 0;
    /**
     * Implementing a visit for a chest
     * @param chest chest
     */
    virtual void visitChest(Chest* chest) = 0;
    /**
     * Implementing a visit for a decoration
     * @param decoration decoration
     */
    virtual void visitDecoration(Decoration* decoration) = 0;
};
