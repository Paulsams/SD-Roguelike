#include "WorldSystem/RandomGeneratorWorldBuilder.h"
#include "Pathfinder/Graph.h"
#include "Pathfinder/PathfinderAStar.h"
#include "Utils/TMXGenerator.h"

#include <gtest/gtest.h>

#include <memory>
#include <functional>
#include <algorithm>
#include <filesystem>

using namespace cocos2d;


struct GeneratorTestsFixture : public testing::TestWithParam<std::tuple<int, int, int>>
{
    pathfinder::PathfinderAStar finder;
    std::function<bool(const pathfinder::Node*)> isValid = [] (const pathfinder::Node*) {return true;};
    std::shared_ptr<LevelTileConfig> config = std::make_shared<WorldTileConfig>("./Resources/World.json")->getLevelsTileConfig().at(0);
};

INSTANTIATE_TEST_SUITE_P(
        GeneratorTests,
        GeneratorTestsFixture,
        testing::Values(
                std::make_tuple(15, 15, 2),
                std::make_tuple(30, 30, 3),
                std::make_tuple(55, 55, 4),
                std::make_tuple(110, 110, 5)
        ));

TEST_P(GeneratorTestsFixture, generatorTest)
{
    int width = std::get<0>(GetParam());
    int height = std::get<1>(GetParam());
    int iterCount = std::get<2>(GetParam());

    cocos2d::TMXMapInfo* mapInfo = RandomGeneratorWorldBuilder()
                        .setConfig(config)
                        .setWidth(width)
                        .setHeight(height)
                        .setIterCount(iterCount)
                        .setPath("./Resources/Template.tmx")
//                        .setPath(std::filesystem::current_path().parent_path() / "Resources"/ "Template.tmx")
                        .build();

    auto itWalls = std::ranges::find_if(mapInfo->getLayers(), [](cocos2d::TMXLayerInfo* info) { return info->_name == "Walls"; });
    uint32_t* walls = (*itWalls)->_tiles;

    auto itBosses = std::ranges::find_if(mapInfo->getObjectGroups(), [](cocos2d::TMXObjectGroup* group) { return group->getGroupName() == "BossMobs"; });
    ASSERT_FALSE((*itBosses)->getObjects().empty());

    pathfinder::Graph graph(cocos2d::Size(width, height), walls, [](uint32_t gid){ return gid == 0 ? TileType::GROUND : TileType::OBSTACLE; });
    std::set<Vec2Int> not_visited;

    Vec2Int start(-1, -1);
    for (int i = 0; i < width; ++i)
    {
        for (int j = 0; j < height; ++j)
        {
            if (start.x == -1 && start.y == -1 && graph.getNodeByPos(i, j)->tile == TileType::GROUND)
            {
                start.x = i;
                start.y = j;
            }
            else if (graph.getNodeByPos(i, j)->tile == TileType::GROUND)
            {
                not_visited.emplace(i, j);
            }
        }
    }

    while (!not_visited.empty())
    {
        Vec2Int end = *not_visited.begin();
        std::vector<Vec2Int> res = finder.findPath(graph, graph.getNodeByPos(start), graph.getNodeByPos(end), isValid);
        ASSERT_FALSE(res.empty());
        std::for_each(res.begin(), res.end(), [&](Vec2Int pos){not_visited.erase(pos);});
    }

    ASSERT_TRUE(not_visited.empty());
}
