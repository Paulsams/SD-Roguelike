#pragma once
#include "AttackData.h"
#include "IAttackSearch.h"

class AttackSearchFromDFS : public IAttackSearch
{
public:
    AttackSearchFromDFS(const std::map<Vec2Int, std::shared_ptr<AttackData>>&)
    {
        // TODO: создать граф
    }
    
    bool isReachable(World* world, Vec2Int startPosition, Vec2Int endPosition) const override
    {
        return true;
    }
};
