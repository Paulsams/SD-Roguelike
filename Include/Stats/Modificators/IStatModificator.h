#pragma once

#include <memory>

struct IStatModificator
{
    virtual ~IStatModificator() = default;
    virtual float modify(float value) const = 0;
    virtual std::shared_ptr<IStatModificator> clone() const = 0;
};
