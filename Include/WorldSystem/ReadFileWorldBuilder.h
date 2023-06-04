#pragma once
#include <string>

#include "2d/CCTMXTiledMap.h"

class ReadFileWorldBuilder
{
public:
    ReadFileWorldBuilder& setPath(const std::string& path);
    cocos2d::TMXTiledMap* build() const;
private:
    std::string m_path;
};
