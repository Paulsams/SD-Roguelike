#pragma once
#include <memory>

#include "Backpack.h"
#include "PlayerInput.h"
#include "GameLoop/IUpdatable.h"
#include "ItemsSystem/BaseItem.h"
#include "WorldSystem/BaseEntity.h"
#include "WorldSystem/FunctionVisitorEntities.h"

class Player : public BaseEntity
{
public:
    static Player* create(World* world);
    
    const std::shared_ptr<IStatsContainer> getStats() const override { return m_statsContainer; }
    
    ObservableVector<BaseItem*>& getInventory() { return m_items; }
    Backpack& getBackpack() { return m_backpack; }

    bool init() override;
    
    void update();
    void acceptVisit(std::shared_ptr<IVisitorEntities> visitor) override { visitor->visitPlayer(this); }

    EventContainer<> attacked;

private:
    Player(World* world);
    
    void onMove(Direction direction);
    void onAttacked();
    void onInteracted();
    
    FunctionHandler<Direction> m_moveDelegate;
    FunctionHandler<> m_attackedDelegate;
    FunctionHandler<> m_interactedDelegate;

    std::shared_ptr<FunctionVisitorEntitiesReturnVoid> m_interactedVisitor;
    
    PlayerInput m_input;
    cocos2d::Sprite* m_helmet = nullptr;
    cocos2d::Sprite* m_clothes = nullptr;
    cocos2d::Sprite* m_templatePeople = nullptr;
    Backpack m_backpack;
    std::shared_ptr<StatsContainer> m_statsContainer;
    ObservableVector<BaseItem*> m_items;

    std::optional<Direction> m_choicedDirection;
};
