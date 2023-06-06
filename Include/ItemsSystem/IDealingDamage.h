#pragma once
#include <memory>

#include <WorldSystem/BaseEntity.h>

struct IDealingDamage
{
    virtual float getDamage(const BaseEntity* otherEntity) const = 0;
};
