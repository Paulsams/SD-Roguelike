#pragma once

struct IAttack
{
    virtual ~IAttack() = default;
    virtual void attack() = 0;
};
