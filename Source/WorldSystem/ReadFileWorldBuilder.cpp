#include "WorldSystem/ReadFileWorldBuilder.h"

ReadFileWorldBuilder& ReadFileWorldBuilder::setPath(const std::string& path)
{
    m_path = path;
    return *this;
}

cocos2d::TMXTiledMap* ReadFileWorldBuilder::build() const
{
    return cocos2d::TMXTiledMap::create(m_path);
}
