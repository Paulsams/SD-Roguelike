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
    
    virtual void visitMob(mob::Mob* mob) = 0;
    virtual void visitPlayer(Player* player) = 0;
    virtual void visitItem(BaseItem* item) = 0;
    virtual void visitChest(Chest* chest) = 0;
    virtual void visitDecoration(Decoration* decoration) = 0;
};
