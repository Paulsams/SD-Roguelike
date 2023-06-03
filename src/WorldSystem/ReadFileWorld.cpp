#include "ReadFileWorld.h"

void ReadFileWorld::changePath(const std::string& path)
{
    m_path = path;
}

cocos2d::TMXTiledMap* ReadFileWorld::generate()
{
    return cocos2d::TMXTiledMap::create(m_path);
}
