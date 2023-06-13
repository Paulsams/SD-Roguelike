#include "../Include/Pathfinder/IPathfindingAlgorithm.h"
#include "../Include/Pathfinder/PathfinderAStar.h"
#include <gtest/gtest.h>
#include <cocos2d.h>

pathfinder::PathfinderAStar finder;
std::function<TileType(int)> fToTileType = [](int tile) {
    return static_cast<TileType>(tile);
};

//TEST(PathfinderSmokeTest, pathfinderSmokeTest) {
//    const int height = 1, width = 5;
//    const cocos2d::Size testSize(width, height);
//    uint32_t tiles1[height * width] = {
//            0, 0, 0, 0, 0,
//    };
//    pathfinder::Graph testGraph(testSize, tiles1, fToTileType);
//
//
//    Vec2Int start(0, 0), end(4, 0);
//    auto res = finder.findPath(testGraph, testGraph.getNodeByPos(start), testGraph.getNodeByPos(end));
//
//    std::vector<Vec2Int> expectedPath1 = {{1, 0},
//                                          {2, 0},
//                                          {3, 0},
//                                          {4, 0}};
//    EXPECT_EQ(expectedPath1.size(), res.size());
//    for (int i = 0; i < expectedPath1.size(); ++i) {
//        EXPECT_EQ(expectedPath1[i], res[i]);
//    }
//
//
//    uint32_t tiles2[height * width] = {
//            0, 0, 1, 0, 0,
//    };
//    pathfinder::Graph testGraph2(testSize, tiles2, fToTileType);
//    auto res2 = finder.findPath(testGraph2, testGraph2.getNodeByPos(start), testGraph2.getNodeByPos(end));
//    EXPECT_EQ(0, res2.size());
//
//
//    uint32_t tiles3[height * width] = {
//            0, 0, 2, 0, 0,
//    };
//    pathfinder::Graph testGraph3(testSize, tiles3, fToTileType);
//    auto res3 = finder.findPath(testGraph3, testGraph3.getNodeByPos(start), testGraph3.getNodeByPos(end));
//    EXPECT_EQ(0, res3.size());
//
//
//    uint32_t tiles4[height * width] = {
//            3, 0, 2, 0, 3,
//    };
//    Vec2Int start2(1, 0), end2(3, 0);
//    pathfinder::Graph testGraph4(testSize, tiles4, fToTileType);
//    auto res4 = finder.findPath(testGraph4, testGraph4.getNodeByPos(start2), testGraph4.getNodeByPos(end2));
//    EXPECT_EQ(0, res4.size());
//}
//
//TEST(PathFinderSimple1PathTest, pathfinderSimple1PathTest) {
//    const int height = 2, width = 5;
//    const cocos2d::Size testSize(width, height);
//    uint32_t tiles1[height * width] = {
//            0, 1, 0, 0, 0,
//            0, 0, 0, 1, 0,
//    };
//    pathfinder::Graph testGraph(testSize, tiles1, fToTileType);
//
//    Vec2Int start(0, 1), end(4, 0);
//    auto res = finder.findPath(testGraph, testGraph.getNodeByPos(start), testGraph.getNodeByPos(end));
//    std::vector<Vec2Int> expectedPath = {{0, 0},
//                                         {1, 0},
//                                         {2, 0},
//                                         {2, 1},
//                                         {3, 1},
//                                         {4, 1},
//                                         {4, 0}};
//
//    EXPECT_EQ(expectedPath.size(), res.size());
//    for (int i = 0; i < expectedPath.size() && i < res.size(); ++i) {
//        EXPECT_EQ(expectedPath[i], res[i]);
//    }
//}
//
//TEST(PathFinderSimple2PathTest, pathfinderSimple2PathTest) {
//    const int height = 5, width = 5;
//    const cocos2d::Size testSize(width, height);
//    uint32_t tiles1[height * width] = {
//            0, 1, 0, 0, 0,
//            0, 0, 0, 1, 0,
//            0, 1, 0, 0, 0,
//            0, 0, 0, 1, 0,
//            0, 1, 0, 0, 0,
//    };
//    pathfinder::Graph testGraph(testSize, tiles1, fToTileType);
//
//    Vec2Int start(0, 4), end(4, 2);
//    auto res = finder.findPath(testGraph, testGraph.getNodeByPos(start), testGraph.getNodeByPos(end));
//    std::vector<Vec2Int> expectedPath = {{0, 3},
//                                         {1, 3},
//                                         {2, 3},
//                                         {2, 2},
//                                         {3, 2},
//                                         {4, 2}};
//
//    EXPECT_EQ(expectedPath.size(), res.size());
//    for (int i = 0; i < expectedPath.size() && i < res.size(); ++i) {
//        EXPECT_EQ(expectedPath[i], res[i]);
//    }
//}

//TEST(PathFinderSimple3PathTest, pathfinderSimple3PathTest) {
//    const int height = 7, width = 7;
//    const cocos2d::Size testSize(width, height);
//    uint32_t tiles1[height * width] = {
//           3, 3, 3, 3, 3, 3, 3,
//           3, 0, 2, 0, 0, 0, 3,
//           3, 0, 0, 0, 2, 0, 3,
//           3, 0, 1, 0, 0, 0, 3,
//           3, 0, 1, 0, 2, 0, 3,
//           3, 0, 0, 0, 0, 0, 3,
//           3, 3, 3, 3, 3, 3, 3,
//    };
//    pathfinder::Graph testGraph(testSize, tiles1, fToTileType);
//
//    Vec2Int start(1, 5), end(5, 3);
//    auto res = finder.findPath(testGraph, testGraph.getNodeByPos(start), testGraph.getNodeByPos(end));
//    std::vector<Vec2Int> expectedPath = {{1, 4},
//                                         {2, 4},
//                                         {3, 4},
//                                         {3, 3},
//                                         {4, 3},
//                                         {5, 3}};
//
//    EXPECT_EQ(expectedPath.size(), res.size());
//    for (int i = 0; i < expectedPath.size() && i < res.size(); ++i) {
//        EXPECT_EQ(expectedPath[i], res[i]);
//    }
//}


int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}