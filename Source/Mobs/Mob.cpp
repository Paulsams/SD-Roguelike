#include <utility>

#include "Stats/Modificators/BoundsModificator.h"
#include "Mobs/Mob.h"
#include "UI/StatBar.h"

#include "Stats/Modificators/StatWithModificators.h"

namespace mob
{

Mob* Mob::create(World* world, const MobInfo& info, const std::string& pathToBar, std::shared_ptr<StatsContainer> stats)
{
    auto* mob = new (std::nothrow) Mob(world, info, pathToBar, stats);
    if (mob && mob->init())
    {
        cocos2d::Sprite* sprite = cocos2d::Sprite::create(Paths::toGameTileset, World::getRectFromGid(info.gid - 1));
        sprite->setAnchorPoint(cocos2d::Vec2::ZERO);
        mob->setContentSize(sprite->getContentSize());
        mob->addChild(sprite);

        StatBar* statBar = StatBar::create(nullptr, {40.0f, 20.0f}, pathToBar, mob->m_stats->get(HEALTH));
        statBar->setPosition({(mob->getContentSize().width - statBar->getContentSize().width) * 0.5f, 30.0f});
        mob->addChild(statBar);

        mob->autorelease();
        return mob;
    }
    CC_SAFE_DELETE(mob);
    return nullptr;
}

Mob* Mob::clone() const
{
    Mob* res = create(getWorld(), m_mobInfo, m_pathToBar, std::make_shared<StatsContainer>(*m_stats));
    return res;
}

void Mob::update()
{
    m_mobInfo.strategy->update(this);
    m_mobInfo.behaviour->update(this);
}

void Mob::changeBehaviour(std::shared_ptr<IMobBehaviour> newBehaviour)
{
    m_mobInfo.behaviour = std::move(newBehaviour);
}

const std::shared_ptr<IStatsContainer> Mob::getStats() const
{
    return m_stats;
}

int Mob::getVisionRange() const
{
    return m_mobInfo.visionRange;
}

float Mob::getExperiencePoints() const
{
    return m_mobInfo.experiencePoints;
}

Mob::Mob(World* world, const MobInfo& info, const std::string& pathToBar, std::shared_ptr<StatsContainer> stats)
    : BaseEntity(world)
    , m_stats(stats)
    , m_mobInfo(info)
    , m_pathToBar(pathToBar)
{
    subscribeOnHealthChanged();
}

void Mob::subscribeOnHealthChanged()
{
    m_stats->get(HEALTH)->changed += [this](IStat::currentValue current, IStat::changedValue, IStat::wantedChangeValue)
    {
        if (current <= 0.0f)
            destroyEntity();
    };
}

}
