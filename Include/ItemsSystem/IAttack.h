#pragma once
#include "ItemsSystem/IAttackSearch.h"
#include "WorldSystem/BaseEntity.h"

struct IAttack
{
    virtual ~IAttack() = default;

    void attack(World* world, Vec2Int position);
    // {
    //     const std::set<Vec2Int>& ranges = getRange();
    //     const IAttackSearch& attackSearch = getAttackSearch();
    //
    //     std::shared_ptr<BaseEntity> entity;
    //     for (Vec2Int range : ranges)
    //     {
    //         const Vec2Int endPosition = position + range;
    //         if (world->tryGetEntity(endPosition, entity))
    //         {
    //             
    //         }
    //     }
    // }
    //
    // virtual const std::set<Vec2Int>& getRange() = 0;
    //
    //  bool isPossibleAttack(World* world, Vec2Int position, Vec2Int localPosition)
    //  {
    //      const std::set<Vec2Int>& ranges = getRange();
    //      const IAttackSearch& attackSearch = getAttackSearch();
    //      
    //      if (ranges.contains(localPosition))
    //      {
    //          const Vec2Int endPosition = position + localPosition;
    //          return isPossibleTileFromAttack(world->getTileType(endPosition)) &&
    //              attackSearch.isReachable(world, position, endPosition);
    //      }
    //
    //      return false;
    //  }

protected:
    virtual const IAttackSearch& getAttackSearch() = 0;
    virtual const bool isPossibleTileFromAttack(TileType tileType) = 0;
};
