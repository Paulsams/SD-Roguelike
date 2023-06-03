#pragma once

#include "cocos2d.h"
#include "GameLoop/GameLoop.h"
#include "Player/Player.h"
#include "WorldSystem/World.h"

class GameScene : public cocos2d::Layer
{
public:
    static cocos2d::Scene* createScene();
    virtual bool init(cocos2d::Camera* camera);
    cocos2d::Point getViewPointCenter(cocos2d::Point position);

    static GameScene* create(cocos2d::Camera* camera);
    
private:
    static constexpr float aspectRatio = 1000.0f;
    
    std::shared_ptr<GameLoop> m_gameLoop;

    World* m_world;
    Player* m_player = nullptr;
    cocos2d::Camera* m_camera = nullptr;
};
