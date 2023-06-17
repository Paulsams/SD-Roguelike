#pragma once
#include <string>

#include "Utils/Common.h"

class ReadFileWorldBuilder
{
public:
    ReadFileWorldBuilder& setPath(const std::string& path);
    Tilemap* build() const;
private:
    std::string m_path;
};
