#pragma once

#include <iostream>
#include <zlib.h>
#include "cocos2d.h"
#include "Utils/Common.h"

// The code from this repository is taken as a basis:
// https://github.com/rohinnz/COCOS2D-X-TMXGenerator

// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
// base64.h
std::string base64_encode(unsigned char const* , unsigned int len);
// std::string base64_decode(std::string const& s);

// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
// TMXGenerator.h
std::string generateXMLForTMXTiledMap(Tilemap* map);
