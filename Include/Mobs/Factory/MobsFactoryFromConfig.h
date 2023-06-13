#pragma once

#include "Mobs/Factory/BaseMobAbstractFactory.h"
#include "Mobs/Factory/MobsConfig.h"
#include "UI/StatBar.h"

namespace mob {

    class MobsFactoryFromConfig final : public BaseMobAbstractFactory {
        std::shared_ptr<MobsConfig> m_mobsConfig;
    public:
        explicit MobsFactoryFromConfig(std::shared_ptr<MobsConfig> mobsConfig)
            : m_mobsConfig(mobsConfig) { }
        
        Mob* createNormal(World* world, int gid) override
        {
            return createMob(world, gid, m_mobsConfig->normals);
        }

        Mob* createElite(World* world, int gid) override
        {
            return createMob(world, gid, m_mobsConfig->elites);
        }

        Mob* createBoss(World* world, int gid) override
        {
            return createMob(world, gid, m_mobsConfig->bosses);
        }

        Mob* createPassive(World* world, int gid) override
        {
            return createMob(world, gid, m_mobsConfig->passives);
        }

    private:
        Mob* createMob(World* world, int gid, const std::unordered_map<int, MobInfo>& createMap)
        {
            Mob* mob = Mob::create(world, createSpriteFromGid(world, gid), createMap.at(gid));
            std::shared_ptr<IStat> healthStat;
            if (mob->getStats()->tryGet(HEALTH, healthStat))
            {
                StatBar* statBar = StatBar::create(nullptr, {40.0f, 20.0f},
                                                   Paths::toHealthBar, healthStat);
                statBar->setPosition({(mob->getContentSize().width - statBar->getContentSize().width) * 0.5f, 30.0f});
                mob->addChild(statBar);
            }
            
            return mob;
        }
        
        cocos2d::Sprite* createSpriteFromGid(World* world, int gid)
        {
            cocos2d::Sprite* sprite = cocos2d::Sprite::create(Paths::toGameTileset, world->getRectFromGid(gid - 1));
            sprite->setAnchorPoint(cocos2d::Vec2::ZERO);
            return sprite;
        }
    };
}
