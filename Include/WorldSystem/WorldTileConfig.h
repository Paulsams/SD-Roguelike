#pragma once

#include "json/rapidjson.h"
#include "json/document.h"
#include "json/filereadstream.h"
#include "json/writer.h"
#include "json/ostreamwrapper.h"
#include "json/istreamwrapper.h"

#include <ranges>
#include <string>
#include <vector>
#include <fstream>
#include <utility>
#include <concepts>
#include <filesystem>

template <typename ValueType>
void ReadArray(const rapidjson::Value& jsonArray, std::vector<ValueType>& output) {
    for (auto it = jsonArray.Begin(); it != jsonArray.End(); it++) {
        auto value = it->template Get<ValueType>();
        output.emplace_back(value);
    }
}


/**
 * Level config
 */
class LevelTileConfig
{
public:
    explicit LevelTileConfig(const rapidjson::Value& jsonValue)
    {
        m_name = jsonValue["Title"].GetString();
        ReadArray(jsonValue["Ground"], m_ground);
        ReadArray(jsonValue["RareGround"], m_rareGround);
        ReadArray(jsonValue["Walls"], m_walls);
        ReadArray(jsonValue["RareWalls"], m_rareWalls);
        ReadArray(jsonValue["Decorations"], m_decorations);
        ReadArray(jsonValue["NormalMobs"], m_normalMobs);
        ReadArray(jsonValue["EliteMobs"], m_eliteMobs);
        ReadArray(jsonValue["BossMobs"], m_bossMobs);
        ReadArray(jsonValue["PassiveMobs"], m_passiveMobs);
        ReadArray(jsonValue["Chests"], m_chests);
    }

    const std::string& getName() const { return m_name; }

    const std::vector<int>& getGround() const { return m_ground; }
    const std::vector<int>& getRareGround() const { return m_rareGround; }
    const std::vector<int>& getWalls() const { return m_walls; }
    const std::vector<int>& getRareWalls() const { return m_rareWalls; }

    const std::vector<int>& getDecorations() const { return m_decorations; }

    const std::vector<int>& getNormalMobs() const { return m_normalMobs; }
    const std::vector<int>& getEliteMobs() const { return m_eliteMobs; }
    const std::vector<int>& getBossMobs() const { return m_bossMobs; }
    const std::vector<int>& getPassiveMobs() const { return m_passiveMobs; }

    const std::vector<int>& getChests() const { return m_chests; }

private:
    std::string m_name;

    /// Sprite tile GIDs vectors from tile set for this specific level
    std::vector<int> m_ground;
    std::vector<int> m_rareGround;
    std::vector<int> m_walls;
    std::vector<int> m_rareWalls;

    std::vector<int> m_decorations;

    std::vector<int> m_normalMobs;
    std::vector<int> m_eliteMobs;
    std::vector<int> m_bossMobs;

    std::vector<int> m_passiveMobs;

    std::vector<int> m_chests;
};


/**
 * World config with tiles from tile set
 *
 * Aggregates different level configs
 */
class WorldTileConfig
{
public:
    template <class STR = std::string>
    requires std::constructible_from<std::string, STR>
    WorldTileConfig(STR&& path)
    {
        rapidjson::Document doc;
        std::fstream ifs(std::forward<STR>(path));

        rapidjson::IStreamWrapper isw(ifs);
        doc.ParseStream(isw);

        const rapidjson::Value& jsonLevels = doc["Level"];
        for (auto it = jsonLevels.Begin(); it != jsonLevels.End(); ++it)
            m_levels.emplace_back(std::make_shared<LevelTileConfig>(*it));
    }

    const std::vector<std::shared_ptr<LevelTileConfig>>& getLevelsTileConfig() const { return m_levels; }

private:
    std::vector<std::shared_ptr<LevelTileConfig>> m_levels;
};
