#pragma once

#include "Utils/Common.h"
#include "WorldSystem/WorldTileConfig.h"


#include "2d/CCFastTMXTiledMap.h"
#include "2d/CCTMXTiledMap.h"
#include "2d/CCFastTMXLayer.h"
#include "2d/CCTMXLayer.h"

#include <set>
#include <random>
#include <cstring>

class RandomGeneratorWorldBuilder
{
public:
    RandomGeneratorWorldBuilder()
        : m_rd()
        , m_gen(m_rd())
    {}

    [[nodiscard]] Tilemap* build() const;

    RandomGeneratorWorldBuilder& setPath(const std::string& path)
    {
        m_path = path;
        return *this;
    }

    RandomGeneratorWorldBuilder& setConfig(const std::shared_ptr<LevelTileConfig>& cfg)
    {
        m_config = cfg;
        return *this;
    }

    RandomGeneratorWorldBuilder& setHeight(int64_t height)
    {
        m_height = height;
        return *this;
    }

    RandomGeneratorWorldBuilder& setWidth(int64_t width)
    {
        m_width = width;
        return *this;
    }

    RandomGeneratorWorldBuilder& setIterCount(int64_t iterCount)
    {
        m_iterCount = iterCount;
        return *this;
    }

    RandomGeneratorWorldBuilder& setDiscardByRatio(bool discardByRatio)
    {
        m_discardByRatio = discardByRatio;
        return *this;
    }

    RandomGeneratorWorldBuilder& setRoomVerticalRatio(double widthRatio)
    {
        m_widthRatio = widthRatio;
        return *this;
    }

    RandomGeneratorWorldBuilder& setRoomHorizontalRatio(float heightRatio)
    {
        m_heightRatio = heightRatio;
        return *this;
    }

private:

    enum class RoomType : uint32_t
    {
        NONE = 0,
        NORMAL = 1,
        ELITE = 2,
        BOSS = 3,
        TREASURE = 4,
        SPAWN = 5,
    };

    template <typename ValueType>
    const ValueType& genFromVec(const std::vector<ValueType>& vec) const
    {
        CCASSERT(!vec.empty(), "Try to generate from empty vector");
        return vec.at(cocos2d::random((size_t)0, vec.size() - 1));
    }

    struct Container
    {
        Container() = default;
        Container(Vec2Int pos, int64_t w, int64_t h);
        Container(int64_t x, int64_t y, int64_t w, int64_t h);

        Vec2Int m_pos;
        Vec2Int m_center;
        int64_t m_width = 0;
        int64_t m_height = 0;
    };

    struct Room
    {
        explicit Room(const Container& cont);

        static constexpr long long s_partDiv = 4;

        Container m_cont;

        RoomType m_type = RoomType::NONE;

        std::set<Vec2Int> m_engaged;

        std::vector<Vec2Int> m_normalMobs;
        std::vector<Vec2Int> m_eliteMobs;
        std::vector<Vec2Int> m_bossMobs;
        std::vector<Vec2Int> m_passiveMobs;

        std::vector<Vec2Int> m_decorations;
        std::vector<Vec2Int> m_chests;
    };

    struct Tree
    {
        explicit Tree(const Container& leaf) : m_leaf(leaf) {}

        [[nodiscard]] std::list<Container> getLeafs() const;
        [[nodiscard]] std::list<std::pair<Vec2Int, Vec2Int>> getPaths() const;

        Container m_leaf;
        std::shared_ptr<Tree> m_left;
        std::shared_ptr<Tree> m_right;
    };

    [[nodiscard]] std::pair<Container, Container> randomSplit(const Container& cont) const;
    [[nodiscard]] std::shared_ptr<Tree> splitContainer(const Container& cont, size_t iterCount) const;

    size_t getFarthestRoom(const std::vector<Room>& in, const std::vector<size_t>& base) const;

    /// drawing
    void drawBackground(TilemapLayer* layer) const;
    void drawRooms(TilemapLayer* layer, const std::shared_ptr<Tree>& tree) const;
    void drawCorridors(TilemapLayer* layer, const std::shared_ptr<Tree>& tree) const;

    void drawGround(TilemapLayer* layer, const std::vector<Container>& containers) const;

    [[nodiscard]] std::vector<Room> generateRooms(const std::shared_ptr<Tree>& tree) const;
    [[nodiscard]] std::vector<Container> generateCorridors(const std::shared_ptr<Tree>& tree) const;

    void fillRoomVec2int(Room& room, std::vector<Vec2Int>& mobs, int64_t counter, int64_t tryCounter) const;

    void fillSpawnRoom(Room& room) const;
    void fillBossRooms(std::vector<Room>& rooms, size_t spawnRoom) const;
    void fillSingleBossRoom(Room& room) const;
    void fillNormalRoom(Room& room) const;
    void fillEliteRoom(Room& room) const;
    void fillTreasureRoom(Room& room) const;

    [[nodiscard]] Tilemap* generateWorld() const;

private:
    /*
     *  Elite rom ratio -> 1 - m_normalRoomRatio - m_treasureRoomRatio;
     *  Number of bosses -> m_iterCount;
     *  Spawn room -> the smallest room
     *  Bosses rooms -> the farthest rooms from spawn
    */
    std::random_device m_rd;
    mutable std::mt19937 m_gen;

    double m_normalRoomRatio = 0.7;
    double m_treasureRoomRatio = 0.05;

    double m_minRoomFillBound = 0.05;
    double m_maxRoomFillBound = 0.2;

    double m_normalRoomTreasureMean = 0.5;
    double m_eliteRoomTreasureMean = 1;
    double m_bossRoomTreasureMean = 3;
    double m_treasureRoomTreasureMean = 5;

    double m_normalGroundRatio = 0.7;

    static constexpr int m_meanCorridorWidth = 3;
    static constexpr int m_sigmaCorridorWidth = 1;

    const std::vector<int> m_bonfires = {782, 783, 784, 785, 786, 787, 788, 789};

    double m_widthRatio = 0.45;
    double m_heightRatio = 0.45;

    std::string m_path;

    std::shared_ptr<LevelTileConfig> m_config;

    int64_t m_height = 0;
    int64_t m_width = 0;

    int64_t m_iterCount = 0;

    bool m_discardByRatio = true;
};
