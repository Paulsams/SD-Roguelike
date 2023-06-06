#pragma once

#include "rapidjson/rapidjson.h"
#include "json/document.h"
#include "rapidjson/filereadstream.h"
#include "rapidjson/writer.h"
#include "rapidjson/ostreamwrapper.h"
#include "rapidjson/istreamwrapper.h"

#include <ranges>
#include <string>
#include <vector>
#include <fstream>
#include <utility>
#include <iostream>
#include <concepts>
#include <filesystem>

template <typename ValueType>
void ReadArray(const rapidjson::Value& jsonArray, std::vector<ValueType>& output) {
    for (auto it = jsonArray.Begin(); it != jsonArray.End(); it++) {
        auto value = it->template Get<ValueType>();
        output.emplace_back(value);
    }
}

class LevelTileConfig
{
public:
    explicit LevelTileConfig(const rapidjson::Value& jsonValue)
    {
        m_name = jsonValue["Title"].GetString();
        ReadArray(jsonValue["Floor"], m_floor);
        ReadArray(jsonValue["Rare floor"], m_rareFloor);
        ReadArray(jsonValue["Walls"], m_walls);
        ReadArray(jsonValue["Rare walls"], m_rareWalls);
        ReadArray(jsonValue["Obstacles"], m_obstacles);
        ReadArray(jsonValue["Normal mobs"], m_normalMobs);
        ReadArray(jsonValue["Elite mobs"], m_eliteMobs);
        ReadArray(jsonValue["Boss mobs"], m_bossMobs);
        ReadArray(jsonValue["Passive mobs"], m_passiveMobs);
    }

    [[nodiscard]] const std::string& getName() const { return m_name; }

    [[nodiscard]] const std::vector<int>& getFloor() const { return m_floor; }
    [[nodiscard]] const std::vector<int>& getRareFloor() const { return m_rareFloor; }
    [[nodiscard]] const std::vector<int>& getWalls() const { return m_walls; }
    [[nodiscard]] const std::vector<int>& getRareWalls() const { return m_rareWalls; }

    [[nodiscard]] const std::vector<int>& getObstacles() const { return m_obstacles; }

    [[nodiscard]] const std::vector<int>& getNormalMobs() const { return m_normalMobs; }
    [[nodiscard]] const std::vector<int>& getEliteMobs() const { return m_eliteMobs; }
    [[nodiscard]] const std::vector<int>& getBossMobs() const { return m_bossMobs; }
    [[nodiscard]] const std::vector<int>& getPassiveMobs() const { return m_passiveMobs; }

private:
    std::string m_name;

    /// Vectors of sprite tile GIDs from tile set for this specific level
    std::vector<int> m_floor;
    std::vector<int> m_rareFloor;
    std::vector<int> m_walls;
    std::vector<int> m_rareWalls;

    std::vector<int> m_obstacles;

    std::vector<int> m_normalMobs;
    std::vector<int> m_eliteMobs;
    std::vector<int> m_bossMobs;

    std::vector<int> m_passiveMobs;
};

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

        const rapidjson::Value& jsonLevels = doc["Levels"];
        for (auto it = jsonLevels.Begin(); it != jsonLevels.End(); ++it)
            m_levels.emplace_back(*it);

//        rapidjson::OStreamWrapper out(std::cout);
//        rapidjson::Writer<rapidjson::OStreamWrapper> writer(out);
//        doc.Accept(writer);
//        std::cout << std::endl;
    }

    [[nodiscard]] const std::vector<LevelTileConfig>& getLevelsTileConfig() const
    {
        return m_levels;
    }

private:
    std::vector<LevelTileConfig> m_levels;
};
