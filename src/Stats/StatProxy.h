#pragma once
#include "IStat.h"

class StatProxy final : public IStat
{
public:
    explicit StatProxy(const IStat& other)
        : other(other) { }

    float getValue() const override
    {
        return other.getValue();
    }

private:
    const IStat& other;
};
