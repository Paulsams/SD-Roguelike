#pragma once
#include <string>

#include "Utils/Common.h"

class ReadFileWorldBuilder
{
public:

    /**
     * Set path for tmx map to be load
     * @param path path
     * @return this builder
     */
    ReadFileWorldBuilder& setPath(const std::string& path);

    /**
     * @return world from file
     */
    Tilemap* build() const;
private:
    std::string m_path;
};
