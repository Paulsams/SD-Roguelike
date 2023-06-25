#pragma once
#include <memory>

#include "Utils/EventsSystem.h"
#include "Stats/Modificators/IStatModificator.h"
#include "Utils/Common.h"

class IStat
{
public:
    using currentValue = float;
    using changedValue = float;
    using wantedChangeValue = float;

    EventContainer<currentValue, changedValue, wantedChangeValue> changed; 

    virtual std::shared_ptr<IStat> clone() const = 0;

    virtual ~IStat() = default;
    virtual float getValue() const = 0;
    virtual void changeValueBy(float value) = 0;

    virtual IStat& addModificator(std::shared_ptr<IStatModificator> statModificator) = 0;
    virtual IStat& removeModificator(std::shared_ptr<IStatModificator> statModificator) = 0;
    
    template <Derived<IStatModificator> T>
    bool tryGet(std::shared_ptr<T>& outModificator) const
    {
        for (auto& modificator : getModificators())
        {
            outModificator = std::dynamic_pointer_cast<T>(modificator);
            if (outModificator)
                return true;
        }
        return false;
    }
    
    virtual const std::vector<std::shared_ptr<IStatModificator>>& getModificators() const = 0;
};
