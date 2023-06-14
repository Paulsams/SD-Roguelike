#pragma once
#include <memory>

#include "Backpack.h"
#include "PlayerInput.h"
#include "ItemsSystem/BaseItem.h"
#include "Stats/LevelHandler.h"
#include "WorldSystem/BaseEntity.h"
#include "WorldSystem/FunctionVisitorEntities.h"

class Player : public BaseEntity
{
public:
    enum ClotheType
    {
        HELMET = 0,
        BIB = 1,
        TEMPLATE_PEOPLE = 2,
    };
    
    static Player* create(World* world);

    const std::shared_ptr<IStatsContainer> getStats() const override { return m_statsContainer; }
    
    ObservableVector<BaseItem*>& getInventory() { return m_items; }
    Backpack& getBackpack() { return m_backpack; }

    bool init() override;

    void update();
    void acceptVisit(std::shared_ptr<IVisitorEntities> visitor) override { visitor->visitPlayer(this); }

    void setClothe(ClotheType type, const SpriteWithRect& textureInfo);

    EventContainer<> attacked;

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
    ObservableVector<BaseItem*> m_items;
    
    std::shared_ptr<LevelHandler> m_levelHandler;

    std::optional<Direction> m_choicedDirection;

    std::shared_ptr<FunctionVisitorEntitiesReturnVoid> m_killVisitor;
    FunctionHandler<BaseEntity*, float> m_attackedDelegate;
};
