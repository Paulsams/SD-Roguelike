#pragma once

#include "IStatModificator.h"
#include "Stats/IStat.h"

#include <vector>
#include <ranges>

class StatWithModificators final : public IStat
{
public:
    using modifiedFunction = std::function<float(float, float)>;
    
    StatWithModificators(float startValue);
    
    float getValue() const override;
    void changeValueBy(float value) override;
    IStat& addModificator(std::shared_ptr<IStatModificator> statModificator) override;
    IStat& removeModificator(std::shared_ptr<IStatModificator> statModificator) override;

protected:
    const std::vector<std::shared_ptr<IStatModificator>>& getModificators() const override { return m_modificators; }

private:
    float m_value;
    std::vector<std::shared_ptr<IStatModificator>> m_modificators;
};
