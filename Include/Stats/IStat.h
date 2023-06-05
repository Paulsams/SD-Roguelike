#pragma once
#include <memory>

#include "3d/CCAnimationCurve.h"
#include "Utils/EventsSystem.h"
#include "Stats/Modificators/IStatModificator.h"

template<class T, class U>
concept Derived = std::is_base_of_v<U, T>;

class IStat
{
public:
    using oldValue = float;
    using changedValue = float;
    using currentValue = float;
    
    EventContainer<oldValue, currentValue, currentValue> changed; 
    
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
