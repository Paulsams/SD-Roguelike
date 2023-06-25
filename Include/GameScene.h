#pragma once

#include "GameLoop/GameLoop.h"
#include "Player/Player.h"
#include "UI/Canvas.h"
#include "WorldSystem/World.h"
#include "WorldSystem/WorldTileConfig.h"

#include "cocos2d.h"


/**
 * Scene in the displayed window for user
 */
class GameScene final : public cocos2d::Layer
{
public:
    /**
     * Static method for creating a new instance GameScene
     * @return Returns a pair from the pointer to the created scene and the root node that is nested in it
     */
    static std::pair<cocos2d::Scene*, GameScene*> createScene();
    
    /**
     * Every frame of the screen refresh is called.
     * @param delta time elapsed since the last frame
     */
    void update(float delta) override;

    /**
     * @param position global position to the position relative to the default camera on this scene
     */
    cocos2d::Point getViewPointCenter(cocos2d::Point position) const;
    
    /**
     * An event that notifies that the scene needs to be reloaded
     */
    EventContainer<> restarted;
private:
    void createPlayer();
    
    static GameScene* create(cocos2d::Camera* camera);
    bool init(cocos2d::Camera* camera);
    GameScene();
    
    static constexpr float aspectRatio = 1000.0f;
    static constexpr float cameraZ = 10.0f;

    FunctionHandler<BaseEntity*, BaseEntity::OldPosition, BaseEntity::NewPosition> movedPlayerDelegate;
    FunctionHandler<> attackPlayerDelegate;
    
    std::shared_ptr<GameLoop> m_gameLoop;

    std::shared_ptr<WorldTileConfig> m_worldTileConfig;

    bool m_runningScene = true;
    World* m_world = nullptr;
    Player* m_player = nullptr;
    cocos2d::Camera* m_camera = nullptr;
    Canvas* m_canvas = nullptr;
};
