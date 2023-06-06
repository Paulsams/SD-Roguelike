#pragma once
#include <memory>
#include <WorldSystem/BaseEntity.h>

struct IDealingDamage
{
    float getDamage(std::shared_ptr<const BaseEntity> otherEntity) const;
};
