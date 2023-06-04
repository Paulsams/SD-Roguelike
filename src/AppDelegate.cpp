#include "AppDelegate.h"
#include "GameScene.h"

using namespace cocos2d;

AppDelegate::AppDelegate() {

}

AppDelegate::~AppDelegate() 
{
}

bool AppDelegate::applicationDidFinishLaunching() {
    auto director = Director::getInstance();
    director->setProjection(Director::Projection::_2D);
    if(!director->getOpenGLView()) {
        GLViewImpl* glView = GLViewImpl::create("Roguelike", true);
        glView->setDesignResolutionSize(1280, 720, ResolutionPolicy::SHOW_ALL);
        glView->setWindowed(1280, 720);
        //director->setAnimationInterval(0.01f);
        director->setOpenGLView(glView);
    }

#ifdef _DEBUG
    director->setDisplayStats(true);
#endif

    auto scene = GameScene::createScene();
    director->runWithScene(scene);

    return true;
}

void AppDelegate::applicationDidEnterBackground() {
}

void AppDelegate::applicationWillEnterForeground() {
}