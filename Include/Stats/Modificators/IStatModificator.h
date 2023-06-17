#pragma once

struct IStatModificator
{
    virtual ~IStatModificator() = default;
    virtual float modify(float value) const = 0;
};
