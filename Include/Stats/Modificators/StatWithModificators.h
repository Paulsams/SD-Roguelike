#pragma once

#include "Stats/Modificators/IStatModificator.h"
#include "Stats/IStat.h"

#include <vector>
#include <ranges>


class StatWithModificators final : public IStat
{
public:
    using modifiedFunction = std::function<float(float, float)>;
    
    StatWithModificators(float startValue);

    StatWithModificators(const StatWithModificators& other);
    StatWithModificators(StatWithModificators&& other) = default;

    std::shared_ptr<IStat> clone() const override { return std::make_shared<StatWithModificators>(*this); }

    /**
     * @return stat value
     */
    float getValue() const override;

    /**
     * Change current stats with given value
     * @param value value
     */
    void changeValueBy(float value) override;

    /**
     * Add stat modificator
     * @param statModificator stat modificator
     * @return this stat
     */
    IStat& addModificator(std::shared_ptr<IStatModificator> statModificator) override;

    /**
     * Remove stat modificator
     * @param statModificator stat modificator
     * @return this stat
     */
    IStat& removeModificator(std::shared_ptr<IStatModificator> statModificator) override;

protected:
    const std::vector<std::shared_ptr<IStatModificator>>& getModificators() const override { return m_modificators; }

private:
    float m_value;
    std::vector<std::shared_ptr<IStatModificator>> m_modificators;
};
