#pragma once
#include "IDealingDamage.h"

class SimpleDamage final : public IDealingDamage
{
public:
    SimpleDamage(float value)
        : m_value(value) { }
    
    float getDamage(const BaseEntity*) const override { return m_value; }

private:
    float m_value;
};
