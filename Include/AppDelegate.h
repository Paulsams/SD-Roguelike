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
     * The handler of the entry point to the game
     * @return true    Initialize success, app continue
     * @return false   Initialize failed, app terminate
     */
    virtual bool applicationDidFinishLaunching();
    
    /**
     * Called when the application moves to the background
     */
    virtual void applicationDidEnterBackground();
    
    /**
     * Called when the application reenters the foreground
     */
    virtual void applicationWillEnterForeground();

private:
    static void runNewGame();
};
