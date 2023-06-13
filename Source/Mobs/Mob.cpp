#include <utility>

#include "Stats/Modificators/BoundsModificator.h"
#include "Mobs/Mob.h"

#include "Stats/Modificators/StatWithModificators.h"

namespace mob
{
    Mob* Mob::create(World* world, cocos2d::Sprite* sprite, const MobInfo& info)
    {
        auto* mob = new (std::nothrow) Mob(world, info);
        if (mob && mob->init())
        {
            mob->setContentSize(sprite->getContentSize());
            mob->addChild(sprite);
            mob->autorelease();
            return mob;
        }
        CC_SAFE_DELETE(mob);
        return nullptr;
    }

    void Mob::update() {
        m_strategy->update(this);
        m_behaviour->update(this);
    }

    void Mob::changeBehaviour(std::shared_ptr<IMobBehaviour> newBehaviour)
    {
        m_behaviour = std::move(newBehaviour);
    }

    const std::shared_ptr<IStatsContainer> Mob::getStats() const {
        return m_stats;
    }

    int Mob::getVisionRange() const {
        return m_visionRange;
    }

    Mob::Mob(World* world, const MobInfo& info)
        : BaseEntity(world)
        , m_visionRange(info.visionRange)
        , m_strategy(info.strategy())
        , m_behaviour(info.startBehaviour())
        , m_stats(std::make_shared<StatsContainer>())
    {
        const auto mobHpStat = std::make_shared<StatWithModificators>(info.health);
        mobHpStat->addModificator(std::make_shared<BoundsModificator>(MinMax(0, info.health)));
        m_stats->add(HEALTH, mobHpStat);

        mobHpStat->changed += [this](IStat::oldValue, IStat::currentValue current, IStat::changedValue)
        {
            if (current <= 0.0f)
                destroyEntity();
        };
    }
}
