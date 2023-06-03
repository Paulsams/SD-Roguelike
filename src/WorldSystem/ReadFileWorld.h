#pragma once

#include "IWorldBuilder.h"

class ReadFileWorld : public IWorldBuilder
{
public:
    void changePath(const std::string& path);
    cocos2d::TMXTiledMap* generate() override;

private:
    std::string m_path;
};
