#pragma once
#include "PlayerItemsOnUI.h"
#include "2d/CCNode.h"
#include "GameLoop/GameLoop.h"
#include "WorldSystem/World.h"
#include "Player/Player.h"

class Canvas : public cocos2d::Node
{
public:
    static constexpr float widthRightPanel = 300.0f;
    static constexpr float heightBackpack = 100.0f;
    
    static Canvas* create(World* world, Player* player, std::shared_ptr<GameLoop> gameLoop);

    bool init() override;
    void update(float delta) override;

    void showRestartScreen(const std::string& text, const std::function<void()>& restartCallback);

private:
    Canvas(World* world, Player* player, std::shared_ptr<GameLoop> gameLoop);

    World* m_world;
    Player* m_player;
    std::shared_ptr<GameLoop> m_gameLoop;
    std::shared_ptr<PlayerItemsOnUI> m_allItemsPlayer;
};
