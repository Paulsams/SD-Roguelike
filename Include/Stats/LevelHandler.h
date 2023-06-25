#pragma once

#include "Stats/Modificators/BoundsModificator.h"
#include "Stats/IHaveStats.h"
#include "Stats/IStat.h"
#include "Modificators/StatWithModificators.h"

#include <vector>

/**
 * Level up Info
 */
struct LevelUpInfo
{
    const int pointsNeeded; /// experience points till new level
    const int hpIncrease; /// hp increase with level up
};

/**
 * Level handler class
 *
 * Manages all experience changes of the player
 */
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

    /**
     * Change experience points of the player
     * @param value experience points change
     */
    void changeExperiencePoints(IStat::changedValue value) const
    {
        const size_t currentLevel = std::lround(m_level->getValue());
        if (currentLevel == m_levelsUp.size())
            return;
        
        const float overflowValue = (m_levelPoints->getValue() + value) - m_bounds->bounds.getMax();
        if (overflowValue >= 0.0f)
        {
            m_level->changeValueBy(1);
            if (currentLevel == m_levelsUp.size())
            {
                m_levelPoints->changeValueBy(value);
            }
            else
            {
                const LevelUpInfo& levelUpInfo = m_levelsUp[currentLevel];
                m_bounds->bounds = MinMax(0.0f, levelUpInfo.pointsNeeded);
                // TODO: если получить очков больше, чем нужно для этого уровня + для следующего, то очки сверху потеряются в никуда, так как заклемпятся по максимуму
                m_levelPoints->changeValueBy(-m_levelPoints->getValue() + overflowValue);
                m_levelUpFunc(levelUpInfo);
            }
        } else
        {
            m_levelPoints->changeValueBy(value);
        }
    }

private:
    std::shared_ptr<IStat> m_level;
    std::shared_ptr<IStat> m_levelPoints;
    std::shared_ptr<BoundsModificator> m_bounds;
    std::vector<LevelUpInfo> m_levelsUp;
    std::function<void(const LevelUpInfo&)> m_levelUpFunc;
};
