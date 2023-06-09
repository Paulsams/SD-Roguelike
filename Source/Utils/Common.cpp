#include "Utils/Common.h"

inline const std::string Paths::toUITileset = "Tileset.png";
inline const std::string Paths::toGameTileset = "Tileset.png";
inline const std::string Paths::toIconFrame = "IconFrame.png";

inline const SpriteWithRect Paths::toAllInventoryCell = {toUITileset,
    cocos2d::Rect{cocos2d::Vec2{70 % 64, 70 / 64} * 32, {32, 32}}};

inline const SpriteWithRect Paths::toWeaponInventoryCell = {toUITileset,
    cocos2d::Rect{cocos2d::Vec2{1812 % 64, 1812 / 64} * 32, {32, 32}}};

inline const SpriteWithRect Paths::toAccessoryInventoryCell = {toUITileset,
    cocos2d::Rect{cocos2d::Vec2{1316 % 64, 1316 / 64} * 32, {32, 32}}};

inline const SpriteWithRect Paths::toSpellInventoryCell = {toUITileset,
    cocos2d::Rect{cocos2d::Vec2{1495 % 64, 1495 / 64} * 32, {32, 32}}};

inline const SpriteWithRect Paths::whiteCell = {toUITileset,
    cocos2d::Rect(cocos2d::Vec2{32, 32}, {32, 32})};

inline const cocos2d::Color3B Colors::background{166, 166, 166};
inline const cocos2d::Color3B Colors::midground{100, 100, 100};
inline const cocos2d::Color3B Colors::backgroundForStatBar{130, 130, 130};
inline const cocos2d::Color3B Colors::backgroundForStat{0, 0, 0};
