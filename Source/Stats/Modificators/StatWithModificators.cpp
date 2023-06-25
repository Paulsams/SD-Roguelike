#include "Stats/Modificators/StatWithModificators.h"

StatWithModificators::StatWithModificators(float startValue)
    : m_value(startValue) { }

StatWithModificators::StatWithModificators(const StatWithModificators& other)
    : m_value(other.m_value)
{
    for (const auto& m_modificator : other.m_modificators)
        m_modificators.emplace_back(m_modificator->clone());
}

float StatWithModificators::getValue() const
{
    float value = m_value;
    for (const auto& modificator : m_modificators)
        value = modificator->modify(value);
    return value;
}

void StatWithModificators::changeValueBy(float value)
{
    const float oldValue = m_value;
    m_value += value;
    m_value = getValue();
    changed(m_value, m_value - oldValue, value);
}

IStat& StatWithModificators::addModificator(std::shared_ptr<IStatModificator> statModificator)
{
    m_modificators.push_back(std::move(statModificator));
    return *this;
}

IStat& StatWithModificators::removeModificator(std::shared_ptr<IStatModificator> statModificator)
{
    m_modificators.erase(std::ranges::find(m_modificators, statModificator));
    return *this;
}
