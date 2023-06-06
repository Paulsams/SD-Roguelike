#pragma once

namespace mob
{

class IState
{
public:
    virtual void enable() = 0;
    virtual void disable() = 0;
    virtual ~IState() = default;
};

}
