#pragma once
#include "IDealingDamage.h"

class DelegateDamage final : public IDealingDamage
{
public:
    void setDealingDamage(std::shared_ptr<IDealingDamage> dealingDamage) { m_dealingDamage = dealingDamage; }
    
    float getDamage(const BaseEntity* otherEntity) const override { return m_dealingDamage->getDamage(otherEntity); }
private:
    std::shared_ptr<IDealingDamage> m_dealingDamage;
};
