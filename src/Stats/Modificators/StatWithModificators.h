#pragma once

#include <vector>
#include "IStatModificator.h"
#include "../IStat.h"

class StatWithModificators final : public IStat
{
public:
    float getValue() const override;

private:
    std::vector<IStatModificator> m_modificators;
};
