#pragma once

#include <vector>
#include "IStatModificator.h"
#include "../IStat.h"

class StatWithModificators final : IStat
{
public:
    float getValue() const override;

private:
    std::vector<IStatModificator> m_modificators;
};
