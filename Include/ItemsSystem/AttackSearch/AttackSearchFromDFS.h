#pragma once

// #include "AttackData.h"
#include "IAttackSearch.h"
#include "ItemsSystem/AttackWithDamage.h"

class AttackSearchFromDFS : public IAttackSearch
{
public:
    AttackSearchFromDFS(/*const std::map<Vec2Int, std::shared_ptr<AttackData>>& ranges*/)
    {
        // TODO: создать граф (хотя лучше просто юзануть поиск пути у мира) или что-то ещё придумать
    }
    
    bool isReachable(World* world, Vec2Int startPosition, Vec2Int endPosition) const override
    {
        return true;
    }
};
