#include "WorldSystem/ReadFileWorldBuilder.h"

ReadFileWorldBuilder& ReadFileWorldBuilder::setPath(const std::string& path)
{
    m_path = path;
    return *this;
}

Tilemap* ReadFileWorldBuilder::build() const
{
    return Tilemap::create(m_path);
}
