#pragma once

#include "cocos2d.h"


/**
 * Game itself
 */
class AppDelegate : private cocos2d::Application
{
public:
    AppDelegate();
    virtual ~AppDelegate();

    /**
     * Do this after launch
     */
    virtual bool applicationDidFinishLaunching();

    /**
     * Do this after entering background
     */
    virtual void applicationDidEnterBackground();

    /**
     * Do this before entering background
     */
    virtual void applicationWillEnterForeground();

private:
    static void runNewGame();
};
