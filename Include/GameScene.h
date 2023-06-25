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
    static std::pair<cocos2d::Scene*, GameScene*> createScene();

    static GameScene* create(cocos2d::Camera* camera);

    /**
     * Update GameScene
     */
    void update(float delta) override;

    /**
     * @return view point center
     */
    cocos2d::Point getViewPointCenter(cocos2d::Point position) const;

    /**
     * Init GameScene with camera
     * @param camera
     * @return true if successful, false otherwise
     */
    bool init(cocos2d::Camera* camera);

    /**
     * Do this on exit
     */
    void onExit() override;
    
    EventContainer<> restarted;
private:
    void createPlayer();

    GameScene();
    
    static constexpr float aspectRatio = 1000.0f;
    static constexpr float cameraZ = 10.0f;

    FunctionHandler<BaseEntity*, BaseEntity::oldPosition, BaseEntity::newPosition> movedPlayerDelegate;
    FunctionHandler<> attackPlayerDelegate;
    
    std::shared_ptr<GameLoop> m_gameLoop;

    std::shared_ptr<WorldTileConfig> m_worldTileConfig;

    bool m_runningScene = true;
    World* m_world = nullptr;
    Player* m_player = nullptr;
    cocos2d::Camera* m_camera = nullptr;
    Canvas* m_canvas = nullptr;
};
