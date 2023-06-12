#include "Utils/TMXGenerator.h"

using namespace cocos2d;

// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
// base64.cpp

/*
 base64.cpp and base64.h
 
 Copyright (C) 2004-2008 René Nyffenegger
 
 This source code is provided 'as-is', without any express or implied
 warranty. In no event will the author be held liable for any damages
 arising from the use of this software.
 
 Permission is granted to anyone to use this software for any purpose,
 including commercial applications, and to alter it and redistribute it
 freely, subject to the following restrictions:
 
 1. The origin of this source code must not be misrepresented; you must not
 claim that you wrote the original source code. If you use this source code
 in a product, an acknowledgment in the product documentation would be
 appreciated but is not required.
 
 2. Altered source versions must be plainly marked as such, and must not be
 misrepresented as being the original source code.
 
 3. This notice may not be removed or altered from any source distribution.
 
 René Nyffenegger rene.nyffenegger@adp-gmbh.ch
 
 */

static const std::string base64_chars =
"ABCDEFGHIJKLMNOPQRSTUVWXYZ"
"abcdefghijklmnopqrstuvwxyz"
"0123456789+/";

static inline bool is_base64(unsigned char c) {
    return (isalnum(c) || (c == '+') || (c == '/'));
}

std::string base64_encode(unsigned char const* bytes_to_encode, unsigned int in_len) {
    std::string ret;
    int i = 0;
    int j = 0;
    unsigned char char_array_3[3];
    unsigned char char_array_4[4];
    
    while (in_len--) {
        char_array_3[i++] = *(bytes_to_encode++);
        if (i == 3) {
            char_array_4[0] = (char_array_3[0] & 0xfc) >> 2;
            char_array_4[1] = ((char_array_3[0] & 0x03) << 4) + ((char_array_3[1] & 0xf0) >> 4);
            char_array_4[2] = ((char_array_3[1] & 0x0f) << 2) + ((char_array_3[2] & 0xc0) >> 6);
            char_array_4[3] = char_array_3[2] & 0x3f;
            
            for(i = 0; (i <4) ; i++)
                ret += base64_chars[char_array_4[i]];
            i = 0;
        }
    }
    
    if (i)
    {
        for(j = i; j < 3; j++)
            char_array_3[j] = '\0';
        
        char_array_4[0] = (char_array_3[0] & 0xfc) >> 2;
        char_array_4[1] = ((char_array_3[0] & 0x03) << 4) + ((char_array_3[1] & 0xf0) >> 4);
        char_array_4[2] = ((char_array_3[1] & 0x0f) << 2) + ((char_array_3[2] & 0xc0) >> 6);
        char_array_4[3] = char_array_3[2] & 0x3f;
        
        for (j = 0; (j < i + 1); j++)
            ret += base64_chars[char_array_4[j]];
        
        while(i++ < 3)
            ret += '=';
    }
    
    return ret;
}

// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
// TMXGenerator.cpp

/*
 TMXGenerator.cpp and TMXGenerator.h
 
 Copyright (C) 2013 Rohin Knight
  
 This source code is provided 'as-is', without any express or implied
 warranty. In no event will the author be held liable for any damages
 arising from the use of this software.
 
 Permission is granted to anyone to use this software for any purpose,
 including commercial applications, and to alter it and redistribute it
 freely, subject to the following restrictions:
 
 1. The origin of this source code must not be misrepresented; you must not
 claim that you wrote the original source code. If you use this source code
 in a product, an acknowledgment in the product documentation would be
 appreciated but is not required.
 
 2. Altered source versions must be plainly marked as such, and must not be
 misrepresented as being the original source code.
 
 3. This notice may not be removed or altered from any source distribution.
 
 Rohin Knight rohin.knight@gmail.com
 
 */

std::string generateXMLForTMXTiledMap(Tilemap* map)
{
    std::ostringstream is;

    //
    // -- Lambda function to add properties
    //
    auto tryAddPropertiesToStream = [] (ValueMap& properties, std::ostringstream& is)
    {
        if (properties.size() == 0)
            return;
        
        is << "<properties>" << std::endl;
        
        for (auto [name, value] : properties)
            is << "<property name=\"" << name << "\" value=\"" << value.asString() << "\" />" << std::endl;
        
        is << "</properties>" << std::endl;        
    };
    
    //
    // -- Map XML
    //
    {
        std::string orientation = "orthogonal";   // TODO: Handle different orientations
        
        is << "<?xml version=\"1.0\" encoding=\"UTF-8\"?>" << std::endl;
        is << "<map version=\"1.10\" orientation=\"" << orientation
            << "\" width=\"" << static_cast<int>(map->getMapSize().width) << "\" height=\"" << static_cast<int>(map->getMapSize().height)
            << "\" tilewidth=\"" << static_cast<int>(map->getTileSize().width) << "\" tileheight=\"" << static_cast<int>(map->getTileSize().height) << "\">" << std::endl;
        
        tryAddPropertiesToStream(map->getProperties(), is);
    }
    //
    // -- Tile Sets to XML
    //
    {
        std::unordered_map<std::string, TMXTilesetInfo*> tilesets;
        
        for (auto& obj : map->getChildren())
        {
            if(auto layer = dynamic_cast<TilemapLayer*>(obj))
            {
                TMXTilesetInfo* tilesetInfo = layer->getTileSet();
                tilesets.insert({tilesetInfo->_name, tilesetInfo});
            }
        }
        
        for (auto [name, tilesetInfo] : tilesets)
        {
            is  << "<tileset firstgid=\"" << tilesetInfo->_firstGid << "\" name=\"" << tilesetInfo->_name
                << "\" tilewidth=\"" << static_cast<int>(tilesetInfo->_tileSize.width) << "\" tileheight=\"" << static_cast<int>(tilesetInfo->_tileSize.height)
                << "\" spacing=\"" << tilesetInfo->_spacing << "\" margin=\"" << tilesetInfo->_margin << "\">" << std::endl;
            
            
            // NOTE: This only stores the filename.
            
            std::string path(tilesetInfo->_sourceImage);
            std::string filename = path.substr(path.find_last_of("\\/") + 1);
            
            is  << "<image source=\"" << filename //tilesetInfo->m_sSourceImage
                << "\" width=\"" << tilesetInfo->_imageSize.width << "\" height=\"" << tilesetInfo->_imageSize.height << "\" />" << std::endl;
            
            is  << "</tileset>" << std::endl;
        }
    }
    //
    // -- Layers to XML
    //
    {
        for (auto& obj : map->getChildren())
        {
            TMXLayer* layer = dynamic_cast<TMXLayer*>(obj);
            if (layer)
            {
                int layerWidth = static_cast<int>(layer->getLayerSize().width);
                int layerHeight = static_cast<int>(layer->getLayerSize().height);
                
                is  << "<layer name=\"" << layer->getLayerName() << "\" width=\"" << layerWidth << "\" height=\"" << layerHeight
                    << "\" visible=\"" << static_cast<int>(layer->isVisible()) << "\">" << std::endl;
                
                tryAddPropertiesToStream(layer->getProperties(), is);

                std::string encoding = "base64";   // TODO: Handle different encoding
                std::string compression = "zlib";  // TODO: Handle different compression
                
                is << "<data encoding=\"" << encoding << "\" compression=\"" << compression << "\">" << std::endl;
                
                
                unsigned int* tiles = layer->getTiles();
                
                //
                // -- Compress with zlib
                //
                uLong ucompSize = (uLong)(layerWidth * layerHeight * sizeof(unsigned int));
                uLong* buffer = new uLong[ucompSize];
                uLong compSize = compressBound(ucompSize);
                
                // Deflate
                compress((Bytef *)buffer, &compSize, (Bytef *)tiles, ucompSize);
                
                // -- Base64 Encode
                std::string encoded = base64_encode((unsigned char*)buffer, (unsigned int)compSize);
                is << encoded << std::endl;
                
                delete [] buffer;
                
                is << "</data>" << std::endl;
                
                // TODO: Handle rotation data
                
                is << "</layer>" << std::endl;
            }
        }
    }
    //
    // -- Object Groups to XML
    //
    {
        for (auto& objectGroup : map->getObjectGroups())
        {
            CCAssert(objectGroup != NULL, "Invalid object group!");
            
            int mapWidth = static_cast<int>(map->getMapSize().width);    // TODO: Get width and height from object group
            int mapHeight = static_cast<int>(map->getMapSize().height);
            
            is << "<objectgroup name=\"" << objectGroup->getGroupName() << "\" width=\"" << mapWidth << "\" height=\"" << mapHeight << "\">";
            tryAddPropertiesToStream(objectGroup->getProperties(), is);

            is << std::endl;
            
            for (auto& obj : objectGroup->getObjects())
            {
                is << "<object";
                
                for (auto& [key, value] : obj.asValueMap())
                {
                    is << " " << key << "=\"";
                    
                    if (key == "y")  // Convert back to Tiled coordinate system
                        is << (mapHeight - 1) * map->getTileSize().height - value.asFloat() << "\"";
                    else if (value.getType() == Value::Type::FLOAT || value.getType() == Value::Type::DOUBLE)
                        is << value.asFloat() << "\"";
                    else
                        is << value.asString() << "\"";
                    
                    //is << " " << key << "=\"" << value << "\"";
                }
                
                is << " />" << std::endl;
            }
            
            is << "</objectgroup>" << std::endl;
        }
    }
    //
    // --
    //
    is << "</map>";
    return is.str();
}