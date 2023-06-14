#pragma once
#include <Stats/Modificators/BoundsModificator.h>
#include "Stats/IHaveStats.h"
#include "Stats/IStat.h"

#include <vector>

#include "Modificators/StatWithModificators.h"

struct LevelUpInfo
{
    const int pointsNeeded;
    const int hpIncrease;
};

class LevelHandler
{
public:
    LevelHandler(const std::shared_ptr<StatsContainer>& stats, std::vector<LevelUpInfo>&& levels,
                 std::function<void(const LevelUpInfo&)> levelUpFunc)
        : m_levelsUp(std::move(levels))
        , m_levelUpFunc(std::move(levelUpFunc))
    {
        m_level = std::make_shared<StatWithModificators>(1);
        stats->add(LEVEL, m_level);
    
        m_levelPoints = std::make_shared<StatWithModificators>(0.0f);
        m_bounds = std::make_shared<BoundsModificator>(MinMax(0, 0));
        m_levelPoints->addModificator(m_bounds);
        stats->add(LEVEL_POINTS, m_levelPoints);
        
        m_bounds->bounds = MinMax(0.0f, m_levelsUp[0].pointsNeeded);
    }
    
    void changeExperiencePoints(IStat::changedValue changedValue) const
    {
        if (m_level->getValue() == m_levelsUp.size())
            return;
        
        const float overflowValue = (m_levelPoints->getValue() + changedValue) - m_bounds->bounds.getMax();
        if (overflowValue >= 0.0f)
        {
            m_level->changeValueBy(1);
            if (m_level->getValue() == m_levelsUp.size())
            {
                m_levelPoints->changeValueBy(changedValue);
            }
            else
            {
                const LevelUpInfo& levelUpInfo = m_levelsUp[m_level->getValue()];
                m_bounds->bounds = MinMax(0.0f, levelUpInfo.pointsNeeded);
                m_levelPoints->changeValueBy(-m_levelPoints->getValue() + overflowValue);
                m_levelUpFunc(levelUpInfo);
            }
        } else
        {
            m_levelPoints->changeValueBy(changedValue);
        }
    }

private:
    std::shared_ptr<IStat> m_level;
    std::shared_ptr<IStat> m_levelPoints;
    std::shared_ptr<BoundsModificator> m_bounds;
    std::vector<LevelUpInfo> m_levelsUp;
    std::function<void(const LevelUpInfo&)> m_levelUpFunc;
};
