#pragma once

#include "Utils/EventsSystem.h"
#include "Stats/Modificators/IStatModificator.h"
#include "Utils/Common.h"

#include <memory>


class IStat
{
public:
    using currentValue = float;
    using changedValue = float;
    using wantedChangeValue = float;

    EventContainer<currentValue, changedValue, wantedChangeValue> changed; /// changed events


    /**
     * @return clone of this stat
     */
    virtual std::shared_ptr<IStat> clone() const = 0;

    virtual ~IStat() = default;

    /**
     * @return current stat value
     */
    virtual float getValue() const = 0;

    /**
     * Change current stat by value
     * @param value value
     */
    virtual void changeValueBy(float value) = 0;

    /**
     * Add stat modificator
     * @param statModificator stat modificator
     * @return this stat
     */
    virtual IStat& addModificator(std::shared_ptr<IStatModificator> statModificator) = 0;

    /**
     * Remove stat modificator
     * @param statModificator stat modificator
     * @return this stat
     */
    virtual IStat& removeModificator(std::shared_ptr<IStatModificator> statModificator) = 0;

    /**
     * @param outModificator Found modificator
     * @return true if modificator found and changed successfully, false otherwise
     */
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
