#pragma once
#include <Stats/Modificators/BoundsModificator.h>

#include "BaseItem.h"
#include "Player/Player.h"
#include "Stats/StatType.h"

class ConsumableStatItem : public BaseItem
{
public:
    ConsumableStatItem(const cocos2d::Rect& rect, StatType statType, float value)
        : BaseItem(rect)
        , m_statType(statType)
        , m_value(value) { }

    bool interact() override
    {
        std::shared_ptr<IStat> stat;
        if (getPlayer()->getStats()->tryGet(m_statType, stat))
        {
            std::shared_ptr<BoundsModificator> boundsModificator;
            if (stat->tryGet(boundsModificator))
            {
                if (stat->getValue() == boundsModificator->bounds.getMax())
                    return false;
            }

            stat->changeValueBy(m_value);
            return true;
        }
        
        return false;
    }

private:
    StatType m_statType;
    float m_value;
};
