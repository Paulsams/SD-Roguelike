#pragma once

#include "cocos2d.h"
#include "GameLoop/GameLoop.h"
#include "Player/Player.h"
#include "UI/Canvas.h"
#include "WorldSystem/World.h"
#include "WorldSystem/WorldTileConfig.h"

class GameScene : public cocos2d::Layer
{
public:
    static cocos2d::Scene* createScene();
    virtual bool init(cocos2d::Camera* camera);
    cocos2d::Point getViewPointCenter(cocos2d::Point position) const;

    static GameScene* create(cocos2d::Camera* camera);

private:
    void createPlayer();
    
    static constexpr float aspectRatio = 1000.0f;
    static constexpr float cameraZ = 10.0f;
    
    std::shared_ptr<GameLoop> m_gameLoop;

    std::shared_ptr<WorldTileConfig> m_worldTileConfig;

    World* m_world = nullptr;
    Player* m_player = nullptr;
    cocos2d::Camera* m_camera = nullptr;
    Canvas* m_canvas = nullptr;
};
