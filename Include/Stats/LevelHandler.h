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
        : m_levels(std::move(levels))
        , m_levelUpFunc(std::move(levelUpFunc))
    {
        m_levelStat = std::make_shared<StatWithModificators>(1);
        stats->add(LEVEL, m_levelStat);
    
        m_levelPointsStat = std::make_shared<StatWithModificators>(0.0f);
        m_boundsModificator = std::make_shared<BoundsModificator>(MinMax(0, 0));
        m_levelPointsStat->addModificator(m_boundsModificator);
        stats->add(LEVEL_POINTS, m_levelPointsStat);
        
        m_boundsModificator->bounds = MinMax(0.0f, m_levels[0].pointsNeeded);
    }
    
    void changeExperiencePoints(IStat::changedValue changedValue) const
    {
        if (m_levelStat->getValue() == m_levels.size())
            return;
        
        const float overflowValue = (m_levelPointsStat->getValue() + changedValue) - m_boundsModificator->bounds.getMax();
        if (overflowValue >= 0.0f)
        {
            m_levelStat->changeValueBy(1);
            if (m_levelStat->getValue() == m_levels.size())
            {
                m_levelPointsStat->changeValueBy(changedValue);
            }
            else
            {
                const LevelUpInfo& levelUpInfo = m_levels[m_levelStat->getValue()];
                m_boundsModificator->bounds = MinMax(0.0f, levelUpInfo.pointsNeeded);
                m_levelPointsStat->changeValueBy(-m_levelPointsStat->getValue() + overflowValue);
                m_levelUpFunc(levelUpInfo);
            }
        } else
        {
            m_levelPointsStat->changeValueBy(changedValue);
        }
    }

private:
    std::shared_ptr<IStat> m_levelStat;
    std::shared_ptr<IStat> m_levelPointsStat;
    std::shared_ptr<BoundsModificator> m_boundsModificator;
    std::vector<LevelUpInfo> m_levels;
    std::function<void(const LevelUpInfo&)> m_levelUpFunc;
};
