#pragma once

#include "Player/Backpack.h"
#include "Player/PlayerInput.h"
#include "ItemsSystem/BaseItem.h"
#include "Stats/LevelHandler.h"
#include "WorldSystem/BaseEntity.h"
#include "WorldSystem/FunctionVisitorEntities.h"

#include <memory>

/**
 * Player class
 */
class Player : public BaseEntity
{
public:
    enum ClotheType
    {
        HELMET = 0,
        BIB = 1,
        TEMPLATE_PEOPLE = 2,
    };

    /**
     * Create player in the given world
     * @param world world
     * @return player
     */
    static Player* create(World* world);
    ~Player();

    /**
     * @return player stats
     */
    const std::shared_ptr<IStatsContainer> getStats() const override { return m_statsContainer; }

    /**
     * @return player items
     */
    std::shared_ptr<ObservableVector<BaseItem*>> getInventory() { return m_items; }

    /**
     * @return player backpack
     */
    Backpack& getBackpack() { return m_backpack; }

    /**
     * Throw all player items on the ground
     */
    void throwAllItems();

    /**
     * Init player
     * @return true if successful, false otherwise
     */
    bool init() override;

    /**
     * Update player
     */
    void update();

    /**
     * Accept visitor with given function
     * @param visitor visitor
     */
    void acceptVisit(std::shared_ptr<IVisitorEntities> visitor) override { visitor->visitPlayer(this); }

    /**
     * Dress player model
     * @param type clothe type
     * @param textureInfo clothe pixel image with name
     */
    void setClothe(ClotheType type, const SpriteWithRect& textureInfo);

    EventContainer<> attacked; /// attack events

private:
    Player(World* world);
    
    void createClothe(ClotheType type);
    void scheduleDamageIndicators(bool isForcedUpdate = false) const;
    
    void onMove(Direction direction);
    void onAttacked();
    void onInteracted();
    
    FunctionHandler<Direction> m_moveDelegate;
    FunctionHandler<> m_attackedInputDelegate;
    FunctionHandler<> m_interactedDelegate;

    PlayerInput m_input;
    std::unordered_map<ClotheType, cocos2d::Sprite*> m_clothes;
    Backpack m_backpack;
    std::shared_ptr<StatsContainer> m_statsContainer;
    std::shared_ptr<ObservableVector<BaseItem*>> m_items;
    
    std::shared_ptr<LevelHandler> m_levelHandler;

    std::optional<Direction> m_choicedDirection;

    std::shared_ptr<FunctionVisitorEntitiesReturnVoid> m_killVisitor;
    FunctionHandler<BaseEntity*, float> m_attackedDelegate;
};
