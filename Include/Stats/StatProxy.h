#pragma once
#include "IStat.h"

class StatProxy final : public IStat
{
public:
    explicit StatProxy(std::shared_ptr<IStat> other)
        : other(std::move(other)) { }

    float getValue() const override
    {
        return other->getValue();
    }

private:
    const std::shared_ptr<IStat> other;
};
