#include "WorldSystem/RandomGeneratorWorldBuilder.h"

Tilemap* RandomGeneratorWorldBuilder::build() const
{
    if (m_path.empty()
        || m_height == 0
        || m_width == 0
        || m_iterCount == 0
        || std::pow(2, m_iterCount) > m_width
        || std::pow(2, m_iterCount) > m_height
        || !m_config)
    {
        return nullptr;
    }

    return generateWorld();
}

std::list<RandomGeneratorWorldBuilder::Container> RandomGeneratorWorldBuilder::Tree::getLeafs() const
{
    if (!left && !right)
        return {leaf};
    std::list<Container> res = left->getLeafs();
    res.splice(res.end(), right->getLeafs());
    return res;
}

std::list<std::pair<Vec2Int, Vec2Int>> RandomGeneratorWorldBuilder::Tree::getPaths() const
{
    if (!left && !right)
        return {};
    std::list<std::pair<Vec2Int, Vec2Int>> res = {std::make_pair(left->leaf.center, right->leaf.center)};
    res.splice(res.end(), left->getPaths());
    res.splice(res.end(), right->getPaths());
    return res;
}

std::pair<RandomGeneratorWorldBuilder::Container, RandomGeneratorWorldBuilder::Container> RandomGeneratorWorldBuilder::randomSplit(const Container& cont) const
{
    Container c1;
    Container c2;
    if (cocos2d::random() % 2 == 0)
    {
        // Vertical
        c1 = Container{cont.point, cocos2d::random((int64_t) 1, cont.w), cont.h};
        c2 = Container{cont.point.x + c1.w, cont.point.y, cont.w - c1.w, cont.h};

        if (m_discardByRatio)
        {
            double c1WR = static_cast<double>(c1.w) / c1.h;
            double c2WR = static_cast<double>(c2.w) / c2.h;
            if (c1WR < m_widthRatio || c2WR < m_widthRatio)
                return randomSplit(cont);
        }
    }
    else
    {
        // Horizontal
        c1 = Container{cont.point, cont.w, cocos2d::random((int64_t) 1, cont.h)};
        c2 = Container{cont.point.x, cont.point.y + c1.h, cont.w, cont.h - c1.h};

        if (m_discardByRatio)
        {
            double c1HR = static_cast<double>(c1.h) / c1.w;
            double c2HR = static_cast<double>(c2.h) / c2.w;
            if (c1HR < m_heightRatio || c2HR < m_heightRatio)
                return randomSplit(cont);
        }
    }
    return std::make_pair(c1, c2);
}

std::shared_ptr<RandomGeneratorWorldBuilder::Tree> RandomGeneratorWorldBuilder::splitContainer(const Container& cont, size_t iterCount) const
{
    auto root = std::make_shared<Tree>(cont);
    if (iterCount)
    {
        auto [c1, c2] = randomSplit(cont);
        root->left = splitContainer(c1, iterCount - 1);
        root->right = splitContainer(c2, iterCount - 1);
    }
    return root;
}

void RandomGeneratorWorldBuilder::drawBackground(TilemapLayer* layer) const
{
    for (int64_t i = 0; i < m_width; ++i)
        for (int64_t j = 0; j < m_height; ++j)
                layer->setTileGID(1186 + cocos2d::random(0, 6), cocos2d::Vec2(i, j));
//            layer->setTileGID((i + j) % 3050, cocos2d::Vec2(i, j));
}

void RandomGeneratorWorldBuilder::drawPaths(TilemapLayer* layer, const std::shared_ptr<Tree>& tree) const
{
    auto paths = tree->getPaths();
    for (auto [from, to] : paths)
    {
        int8_t alpha = cocos2d::random(1, 4);
        if (from.x == to.x)
        {
            int64_t bound = std::abs(from.y - to.y);
            for (int64_t i = 0; i < bound; ++i)
                for (int8_t j = -alpha/2; j <= alpha/2; ++j)
                    layer->setTileGID(882 + cocos2d::random(0, 3), cocos2d::Vec2(from.x + j, std::min(from.y, to.y) + i));
        }
        else
        {
            int64_t bound = std::abs(from.x - to.x);
            for (int64_t i = 0; i < bound; ++i)
                for (int8_t j = -alpha/2; j <= alpha/2; ++j)
                    layer->setTileGID(882 + cocos2d::random(0, 3), cocos2d::Vec2(std::min(from.x, to.x) + i, from.y + j));
        }
    }
}

void RandomGeneratorWorldBuilder::drawRooms(TilemapLayer* layer, const std::shared_ptr<Tree>& tree) const
{
    std::list<Container> leafs = tree->getLeafs();
    for (const Container& cont : leafs)
    {
        Room room(cont);
        for (int64_t i = 0; i < room.w; ++i)
            for (int64_t j = 0; j < room.h; ++j)
                layer->setTileGID(882 + cocos2d::random(0, 3), cocos2d::Vec2(i + room.point.x, j + room.point.y));
    }
}

Tilemap* RandomGeneratorWorldBuilder::generateWorld() const
{
    Tilemap* tileMap = Tilemap::create(m_path);
    Container mainContainer(0, 0, m_width, m_height);
    std::shared_ptr<Tree> tree = splitContainer(mainContainer, m_iterCount);

    uint32_t* tiles = (uint32_t*)malloc(m_width * m_height * sizeof(uint32_t));
    std::memset(tiles, 0, m_width * m_height * sizeof(uint32_t));

    cocos2d::Size size(m_width, m_height);
    tileMap->setMapSize(size);

    TilemapLayer* layer = tileMap->getLayer("Background");
    void* oldTiles = layer->getTiles();
    free(oldTiles);

    layer->setLayerSize(size);
    layer->setTiles(tiles);

    drawBackground(layer);
    drawPaths(layer, tree);
    drawRooms(layer, tree);

    return tileMap;
}
