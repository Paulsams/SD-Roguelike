#pragma once
#include <memory>

#include "IDealingDamage.h"
#include "ItemsSystem/IAttackSearch.h"
#include "WorldSystem/World.h"
#include "ItemsSystem/IVisualAttack.h"

struct AttackData
{
    std::shared_ptr<IDealingDamage> getDealingDamage() const { return m_dealingDamage; }
    bool isPossibleAttack(TileType tileType) const { return m_possibleAttack(tileType); }
    std::shared_ptr<IVisualAttack> getVisual() const { return m_visualAttack; } 
    std::shared_ptr<IAttackSearch> getSearch() const { return m_attackSearch; }

private:
    std::shared_ptr<IDealingDamage> m_dealingDamage;
    std::function<bool (TileType)> m_possibleAttack;
    std::shared_ptr<IVisualAttack> m_visualAttack;
    std::shared_ptr<IAttackSearch> m_attackSearch;
};
