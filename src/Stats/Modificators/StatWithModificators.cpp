#include "StatWithModificators.h"

float StatWithModificators::getValue() const
{
    float value = m_value;
    for (const IStatModificator& modificator : m_modificators)
        value = modificator.modify(value);
    return value;
}
