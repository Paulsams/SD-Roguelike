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

        /**
         * Spawns a normal mob
         * @param world - game world
         * @param gid - tile id
         * @return pointer to normal Mob
         */
        Mob* createNormal(World* world, int gid) override
        {
            return createMob(world, gid, m_mobsConfig->normals, Paths::toHealthBar);
        }
        /**
        * Spawns a elite mob
        * @param world - game world
        * @param gid - tile id
        * @return pointer to elite Mob
        */
        Mob* createElite(World* world, int gid) override
        {
            return createMob(world, gid, m_mobsConfig->elites, Paths::toHealthBar);
        }
        /**
        * Spawns a boss mob
        * @param world - game world
        * @param gid - tile id
        * @return pointer to boss Mob
        */
        Mob* createBoss(World* world, int gid) override
        {
            return createMob(world, gid, m_mobsConfig->bosses, Paths::toHealthBar);
        }
        /**
        * Spawns a neutral mob
        * @param world - game world
        * @param gid - tile id
        * @return pointer to neutral Mob
        */
        Mob* createPassive(World* world, int gid) override
        {
            return createMob(world, gid, m_mobsConfig->passives, Paths::toExperiencePointsBar);
        }

    private:
        Mob* createMob(World* world, int gid, const std::unordered_map<int, MobInfo>& createMap,
            const std::string& pathToBar)
        {
            Mob* mob = Mob::create(world, createSpriteFromGid(world, gid), createMap.at(gid));
            std::shared_ptr<IStat> healthStat;
            if (mob->getStats()->tryGet(HEALTH, healthStat))
            {
                StatBar* statBar = StatBar::create(nullptr, {40.0f, 20.0f},
                                                   pathToBar, healthStat);
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
