#pragma once

#include <WorldSystem/BaseEntity.h>


/**
 * Damage Modificator class
 */
struct IDamageModificator
{
    virtual ~IDamageModificator() = default;

    /**
     * Modify damage of the given entity
     * @param damage damage value
     * @param entity entity
     * @return modified damage
     */
    virtual float modify(float damage, BaseEntity* entity) const = 0;
};


/**
 * Damage class
 */
class Damage
{
public:
    Damage(float m_damage)
        : m_damage(m_damage)
        , m_modificator(nullptr)
    {}

    /**
     * Set current modificator
     * @param modificator damage modificator
     */
    void changeModificator(const IDamageModificator* modificator) { m_modificator = modificator; }

    /**
     * Remove current modificator
     */
    void resetModificator() { m_modificator = nullptr; }

    /**
     * @param entity entity
     * @return modified damage in float
     */
    float get(BaseEntity* entity) const { return m_modificator ? m_modificator->modify(m_damage, entity) : m_damage; }
        
private:
    float m_damage;
    const IDamageModificator* m_modificator;
};
