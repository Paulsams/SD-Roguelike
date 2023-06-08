#pragma once
#include "IDealingDamage.h"

class DamageFromFunction final : public IDealingDamage
{
public:
    float getDamage(const BaseEntity* otherEntity) const override { return m_func(otherEntity); }

private:
    std::function<float (const BaseEntity* otherEntity)> m_func;
};
