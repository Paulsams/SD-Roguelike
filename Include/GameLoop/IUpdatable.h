#pragma once

struct IUpdatable
{
    virtual ~IUpdatable() = default;
    virtual void update() = 0;
};
