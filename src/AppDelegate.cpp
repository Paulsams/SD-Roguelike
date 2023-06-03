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
        GLViewImpl* glViewImpl = GLViewImpl::create("Roguelike");
        director->setOpenGLView(glViewImpl);
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