#pragma once

#include "Utils/Common.h"
#include "WorldSystem/WorldTileConfig.h"

#include "2d/CCTMXTiledMap.h"
#include "2d/CCTMXLayer.h"

#include <set>
#include <random>
#include <cstring>
#include <unordered_set>


/**
 * Random world generator
 *
 * When build is called generator randomly splits current world rectangle into 2 rectangles
 * If relative ratio of these 2 rectangles is too low then redo current iteration again.
 * This excludes very tight and very wide rectangles.
 *
 * This builds full binary decision tree where leaf rectangles are the base of resulting world rooms.
 *
 * Resulting number of rectangles is 2^iterCount, so after splitting world will have 2^iterCount rooms.
 * Also the number of bosses in the world equals to iterCount.
 *
 * Then every leaf rectangle is randomly stripped from its edges when creating a room.
 * Important!!! Every room must contain its parent rectangle center to connect every room with paths.
 *
 * Then all these rooms are randomly collected in a vector.
 * The first room is assigned to spawn room.
 * Then next iterCount rooms are assigned to be boss rooms.
 * Other rooms are randomly split into normal, elite and treasure rooms.
 */
class RandomGeneratorWorldBuilder
{
private:
    using LayerInfo = cocos2d::TMXLayerInfo;

public:
    RandomGeneratorWorldBuilder() = default;

    /**
     * Build world with current parameters
     * @return world
     */
    cocos2d::TMXMapInfo* build() const;

    /**
     * Set path for template tmx map
     * @param path path
     * @return this builder
     */
    RandomGeneratorWorldBuilder& setPath(const std::string& path)
    {
        m_path = path;
        return *this;
    }

    /**
     * Set world config
     * @param cfg world config
     * @return this builder
     */
    RandomGeneratorWorldBuilder& setConfig(const std::shared_ptr<LevelTileConfig>& cfg)
    {
        m_config = cfg;
        return *this;
    }

    /**
     * Set height of the world
     * @param height height
     * @return this builder
     */
    RandomGeneratorWorldBuilder& setHeight(int height)
    {
        m_height = std::max(16, height);
        return *this;
    }

    /**
     * Set width of the world
     * @param width width
     * @return this builder
     */
    RandomGeneratorWorldBuilder& setWidth(int width)
    {
        m_width = std::max(16, width);
        return *this;
    }

    /**
     * Set iteration count for the world generating algorithm
     * @param iterCount
     * @return
     */
    RandomGeneratorWorldBuilder& setIterCount(int iterCount)
    {
        m_iterCount = std::max(2, iterCount);
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
    static const ValueType& genFromVec(const std::vector<ValueType>& vec)
    {
        return vec.at(cocos2d::random((size_t)0, vec.size() - 1));
    }

    struct Box
    {
        Box() = default;
        Box(Vec2Int pos, int w, int h);
        Box(int x, int y, int w, int h);

        int getArea() const { return width * height; }

        Vec2Int pos;
        Vec2Int center;
        int width = 0;
        int height = 0;
    };

    struct Room
    {
        explicit Room(const Box& _box);

        void createSpawnEnv();
        void createBossEnv();
        void createNormalEnv();
        void createEliteEnv();
        void createTreasureEnv();

        void fillEntities(std::vector<Vec2Int>& mobs, int amount);

        static constexpr int s_partDiv = 5;

        Box box;

        RoomType type = RoomType::NONE;

        std::set<Vec2Int> engaged;
        std::vector<Vec2Int> normalMobs;
        std::vector<Vec2Int> eliteMobs;
        std::vector<Vec2Int> bossMobs;
        std::vector<Vec2Int> passiveMobs;
        std::vector<Vec2Int> decorations;
        std::vector<Vec2Int> chests;
    };

    struct Tree
    {
        explicit Tree(const Box& _leaf) : leaf(_leaf) {}

        [[nodiscard]] std::list<Box> getLeafs() const;
        [[nodiscard]] std::list<std::pair<Vec2Int, Vec2Int>> getPaths() const;

        Box leaf;
        std::shared_ptr<Tree> left;
        std::shared_ptr<Tree> right;
    };

    std::pair<Box, Box> randomSplit(const Box& cont) const;
    std::shared_ptr<Tree> splitContainer(const Box& cont, size_t iterCount) const;

    /// drawing
    void drawWalls(LayerInfo* layer) const;
    void drawGround(LayerInfo* wallsLayer, LayerInfo* groundLayer, const std::vector<Box>& ground) const;

    std::vector<Room> generateRooms(const std::shared_ptr<Tree>& tree) const;
    std::vector<Box> generateCorridors(const std::shared_ptr<Tree>& tree) const;

    cocos2d::TMXMapInfo* generateWorld() const;

    static void fillObjectMetaInfo(cocos2d::ValueMap& object, cocos2d::TMXMapInfo* mapInfo, const Room& room, Vec2Int pos);

private:
    static std::random_device s_rd;
    static std::mt19937 s_gen;

    double m_normalRoomRatio = 0.7;
    double m_treasureRoomRatio = 0.03;

    static constexpr double s_minRoomFillBound = 0.025;
    static constexpr double s_maxRoomFillBound = 0.075;

    static constexpr double s_normalRoomTreasureMean = 3;
    static constexpr double s_eliteRoomTreasureMean = 3.5;
    static constexpr double s_bossRoomTreasureMean = 4.0;
    static constexpr double s_treasureRoomTreasureMean = 5;

    double m_normalTileRatio = 0.75;

    static constexpr int s_meanCorridorWidth = 3.0;
    static constexpr int s_sigmaCorridorWidth = 1.0;

    double m_widthRatio = 0.45;
    double m_heightRatio = 0.45;

    std::string m_path;

    std::shared_ptr<LevelTileConfig> m_config;

    int m_height = 16;
    int m_width = 16;

    int m_iterCount = 2;
};
