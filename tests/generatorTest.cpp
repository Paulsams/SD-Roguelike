#include "WorldSystem/RandomGeneratorWorldBuilder.h"
#include "Pathfinder/Graph.h"
#include "Pathfinder/PathfinderAStar.h"
#include "Utils/TMXGenerator.h"

#include <gtest/gtest.h>

#include <memory>
#include <functional>
#include <filesystem>

using namespace cocos2d;

pathfinder::PathfinderAStar finder;

std::function<bool(const pathfinder::Node*)> isValid = [] (const pathfinder::Node*) {return true;};

struct GeneratorTestsFixture : public testing::TestWithParam<std::tuple<int, int, int>> {
    void SetUp() override
    {
        auto director = Director::getInstance();
        if(!director->getOpenGLView())
        {
            GLViewImpl* glView = GLViewImpl::create("Generator Test", false);
            director->setOpenGLView(glView);
        }
    }

    void TearDown() override {}

    const std::shared_ptr<LevelTileConfig> config = std::make_shared<WorldTileConfig>("../Resources/World.json")->getLevelsTileConfig().at(0);
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

    Tilemap* tileMap = RandomGeneratorWorldBuilder()
                        .setConfig(config)
                        .setWidth(width)
                        .setHeight(height)
                        .setIterCount(iterCount)
                        .setPath((std::filesystem::current_path().parent_path() / "Resources"/ "Template.tmx").string())
                        .build();

    uint32_t* wallsLayer = tileMap->getLayer("Walls")->getTiles();

    const TMXObjectGroup* bossMobsGroup = tileMap->getObjectGroup("BossMobs");
    ASSERT_FALSE(bossMobsGroup->getObjects().empty());

    pathfinder::Graph graph(cocos2d::Size(width, height), wallsLayer, [](uint32_t gid){ return gid == 0 ? TileType::GROUND : TileType::OBSTACLE; });
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
