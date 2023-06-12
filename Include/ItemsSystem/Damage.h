#pragma once

#include <WorldSystem/BaseEntity.h>

struct IDamageModificator
{
    virtual ~IDamageModificator() = default;
    virtual float modify(float damage, BaseEntity* entity) const = 0;
};

class Damage
{
public:
    Damage(float m_damage)
        : m_damage(m_damage)
        , m_modificator(nullptr) { }

    void changeModificator(const IDamageModificator* modificator) { m_modificator = modificator; }
    void resetModificator() { m_modificator = nullptr; }
    
    float get(BaseEntity* entity) const
        { return m_modificator ? m_modificator->modify(m_damage, entity) : m_damage; }
        
private:
    float m_damage;
    const IDamageModificator* m_modificator;
};
