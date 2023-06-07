#include "WorldSystem/RandomGeneratorWorldBuilder.h"

using RGWB = RandomGeneratorWorldBuilder;

Tilemap* RGWB::build() const
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



Tilemap* RGWB::generateWorld() const
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

    std::vector<Room> rooms = generateRooms(tree);

    std::vector<Container> corridors = generateCorridors(tree);
    std::vector<Container> roomsGround(rooms.size());
    for (size_t i = 0; i < rooms.size(); ++i)
        roomsGround[i] = rooms[i].m_cont;

    drawBackground(layer);
//    drawCorridors(layer, tree);
//    drawRooms(layer, tree);

    drawGround(layer, corridors);
    drawGround(layer, roomsGround);

    return tileMap;
}



RGWB::Container::Container(Vec2Int pos, int64_t w, int64_t h)
        : m_pos(pos)
        , m_center(m_pos.x + w/2, m_pos.y + h/2)
        , m_width(w)
        , m_height(h)
{}



RGWB::Container::Container(int64_t x, int64_t y, int64_t w, int64_t h)
        : m_pos(x, y)
        , m_center(m_pos.x + w/2, m_pos.y + h/2)
        , m_width(w)
        , m_height(h)
{}



RGWB::Room::Room(const Container& cont)
{
    int64_t x = cont.m_pos.x + cocos2d::random(1ll, cont.m_width/s_partDiv);
    int64_t y = cont.m_pos.y + cocos2d::random(1ll, cont.m_height/s_partDiv);
    int64_t width = cont.m_width - (x - cont.m_pos.x);
    int64_t height = cont.m_height - (y - cont.m_pos.y);

    width -= cocos2d::random(1ll, width/s_partDiv);
    height -= cocos2d::random(1ll, height/s_partDiv);

    m_cont = Container{x, y, width, height};
}



std::list<RGWB::Container> RGWB::Tree::getLeafs() const
{
    if (!m_left && !m_right)
        return {m_leaf};
    std::list<Container> res = m_left->getLeafs();
    res.splice(res.end(), m_right->getLeafs());
    return res;
}



std::list<std::pair<Vec2Int, Vec2Int>> RGWB::Tree::getPaths() const
{
    if (!m_left && !m_right)
        return {};
    std::list<std::pair<Vec2Int, Vec2Int>> res = {std::make_pair(m_left->m_leaf.m_center, m_right->m_leaf.m_center)};
    res.splice(res.end(), m_left->getPaths());
    res.splice(res.end(), m_right->getPaths());
    return res;
}



std::pair<RGWB::Container, RGWB::Container> RGWB::randomSplit(const Container& cont) const
{
    Container c1;
    Container c2;
    if (cocos2d::random() % 2 == 0)
    {
        // Vertical
        c1 = Container{cont.m_pos, cocos2d::random((int64_t) 1, cont.m_width), cont.m_height};
        c2 = Container{cont.m_pos.x + c1.m_width, cont.m_pos.y, cont.m_width - c1.m_width, cont.m_height};

        if (m_discardByRatio)
        {
            double c1WR = static_cast<double>(c1.m_width) / c1.m_height;
            double c2WR = static_cast<double>(c2.m_width) / c2.m_height;
            if (c1WR < m_widthRatio || c2WR < m_widthRatio)
                return randomSplit(cont);
        }
    }
    else
    {
        // Horizontal
        c1 = Container{cont.m_pos, cont.m_width, cocos2d::random((int64_t) 1, cont.m_height)};
        c2 = Container{cont.m_pos.x, cont.m_pos.y + c1.m_height, cont.m_width, cont.m_height - c1.m_height};

        if (m_discardByRatio)
        {
            double c1HR = static_cast<double>(c1.m_height) / c1.m_width;
            double c2HR = static_cast<double>(c2.m_height) / c2.m_width;
            if (c1HR < m_heightRatio || c2HR < m_heightRatio)
                return randomSplit(cont);
        }
    }
    return std::make_pair(c1, c2);
}



std::shared_ptr<RGWB::Tree> RGWB::splitContainer(const Container& cont, size_t iterCount) const
{
    auto root = std::make_shared<Tree>(cont);
    if (iterCount)
    {
        auto [c1, c2] = randomSplit(cont);
        root->m_left = splitContainer(c1, iterCount - 1);
        root->m_right = splitContainer(c2, iterCount - 1);
    }
    return root;
}



void RGWB::fillRoomVec2int(Room& room, std::vector<Vec2Int>& mobs, int64_t counter, int64_t tryCounter) const
{
    for (int64_t i = 0; i < counter; ++i)
    {
        Vec2Int pos;
        do {
            pos.x = cocos2d::random((int64_t)0, room.m_cont.m_width);
            pos.y = cocos2d::random((int64_t)0, room.m_cont.m_height);
            --tryCounter;
        } while (room.m_engaged.contains(pos) && tryCounter);
        mobs.push_back(pos);
        room.m_engaged.insert(pos);
    }
}



void RGWB::fillSpawnRoom(Room& room) const
{
    room.m_type = RoomType::SPAWN;
    room.m_decorations.emplace_back(room.m_cont.m_pos);
    room.m_decorations.emplace_back(Vec2Int{room.m_cont.m_pos.x, room.m_cont.m_pos.y + room.m_cont.m_height});
    room.m_decorations.emplace_back(Vec2Int{room.m_cont.m_pos.x + room.m_cont.m_width, room.m_cont.m_pos.y});
    room.m_decorations.emplace_back(Vec2Int{room.m_cont.m_pos.x + room.m_cont.m_width, room.m_cont.m_pos.y + room.m_cont.m_height});

    room.m_engaged.insert(room.m_cont.m_center);
    room.m_engaged.insert(room.m_cont.m_pos);
    room.m_engaged.insert(Vec2Int{room.m_cont.m_pos.x, room.m_cont.m_pos.y + room.m_cont.m_height});
    room.m_engaged.insert(Vec2Int{room.m_cont.m_pos.x + room.m_cont.m_width, room.m_cont.m_pos.y});
    room.m_engaged.insert(Vec2Int{room.m_cont.m_pos.x + room.m_cont.m_width, room.m_cont.m_pos.y + room.m_cont.m_height});
}



void RGWB::fillBossRooms(std::vector<Room>& rooms, size_t spawnRoom) const
{
    int64_t bossCount = m_iterCount;
    std::vector<size_t> bossRooms = {spawnRoom};
    for (int64_t i = 0; i < bossCount; ++i)
    {
        bossRooms.emplace_back(getFarthestRoom(rooms, bossRooms));
    }

    for (size_t roomIdx : bossRooms)
    {
        fillSingleBossRoom(rooms[roomIdx]);
    }
}



void RGWB::fillSingleBossRoom(Room& room) const
{
    static std::exponential_distribution<> d(m_bossRoomTreasureMean);
    room.m_type = RoomType::BOSS;
    room.m_bossMobs.push_back(room.m_cont.m_center);
    room.m_engaged.insert(room.m_cont.m_center);

    int64_t roomArea = room.m_cont.m_width * room.m_cont.m_height;
    int64_t tryCounter = m_width * m_height;

    int64_t normalMobsCount = roomArea * m_minRoomFillBound;
    fillRoomVec2int(room, room.m_normalMobs, normalMobsCount, tryCounter);

    int64_t decorationsCount = roomArea * m_minRoomFillBound;
    fillRoomVec2int(room, room.m_decorations, decorationsCount, tryCounter);

    int64_t chestsCount = d(m_gen);
    fillRoomVec2int(room, room.m_chests, chestsCount, tryCounter);
}



void RGWB::fillNormalRoom(Room& room) const
{
    static std::exponential_distribution<> d(m_normalRoomTreasureMean);
    room.m_type = RoomType::NORMAL;
    int64_t roomArea = room.m_cont.m_width * room.m_cont.m_height;
    int64_t tryCounter = m_width * m_height;

    int64_t passiveMobsCount = roomArea * m_minRoomFillBound;
    fillRoomVec2int(room, room.m_passiveMobs, passiveMobsCount, tryCounter);

    int64_t normalMobsCount = roomArea * cocos2d::random(m_minRoomFillBound, m_maxRoomFillBound);
    fillRoomVec2int(room, room.m_normalMobs, normalMobsCount, tryCounter);

    int64_t decorationsCount = roomArea * cocos2d::random(m_minRoomFillBound, m_maxRoomFillBound);
    fillRoomVec2int(room, room.m_decorations, decorationsCount, tryCounter);

    int64_t chestsCount = d(m_gen);
    fillRoomVec2int(room, room.m_chests, chestsCount, tryCounter);
}



void RGWB::fillEliteRoom(Room& room) const
{
    static std::exponential_distribution<> d(m_eliteRoomTreasureMean);
    room.m_type = RoomType::ELITE;
    int64_t roomArea = room.m_cont.m_width * room.m_cont.m_height;
    int64_t tryCounter = m_width * m_height;

    int64_t passiveMobsCount = roomArea * m_minRoomFillBound;
    fillRoomVec2int(room, room.m_passiveMobs, passiveMobsCount, tryCounter);

    int64_t eliteMobsCount = roomArea * m_minRoomFillBound;
    fillRoomVec2int(room, room.m_eliteMobs, eliteMobsCount, tryCounter);

    int64_t normalMobsCount = roomArea * cocos2d::random(m_minRoomFillBound, m_maxRoomFillBound);
    fillRoomVec2int(room, room.m_normalMobs, normalMobsCount, tryCounter);

    int64_t decorationsCount = roomArea * cocos2d::random(m_minRoomFillBound, m_maxRoomFillBound);
    fillRoomVec2int(room, room.m_decorations, decorationsCount, tryCounter);

    int64_t chestsCount = d(m_gen);
    fillRoomVec2int(room, room.m_chests, chestsCount, tryCounter);
}



void RGWB::fillTreasureRoom(Room& room) const
{
    static std::exponential_distribution<> d(m_treasureRoomTreasureMean);
    room.m_type = RoomType::TREASURE;
    int64_t roomArea = room.m_cont.m_width * room.m_cont.m_height;
    int64_t tryCounter = m_width * m_height;

    int64_t passiveMobsCount = roomArea * m_minRoomFillBound;
    fillRoomVec2int(room, room.m_passiveMobs, passiveMobsCount, tryCounter);

    int64_t decorationsCount = roomArea * m_maxRoomFillBound;
    fillRoomVec2int(room, room.m_decorations, decorationsCount, tryCounter);

    int64_t chestsCount = d(m_gen);
    fillRoomVec2int(room, room.m_chests, chestsCount, tryCounter);
}



std::vector<RGWB::Room> RGWB::generateRooms(const std::shared_ptr<Tree>& tree) const
{
    std::list<Container> leafs = tree->getLeafs();
    std::vector<Room> rooms;
    rooms.reserve(leafs.size());

    size_t spawnRoomIdx = 0;
    size_t currIdx = 0;
    size_t smallestArea = -1;
    for (const Container& c : leafs)
    {
        rooms.emplace_back(c);
        if (smallestArea > rooms.back().m_cont.m_width * rooms.back().m_cont.m_height)
        {
            spawnRoomIdx = currIdx;
            smallestArea = rooms.back().m_cont.m_width * rooms.back().m_cont.m_height;
        }
        ++currIdx;
    }

    fillSpawnRoom(rooms[spawnRoomIdx]);
    fillBossRooms(rooms, spawnRoomIdx);

    for (Room& currRoom : rooms)
    {
        if (currRoom.m_type == RoomType::NONE)
        {
            double alpha = cocos2d::random(0., 1.);
            if (alpha < m_normalRoomRatio)
                fillNormalRoom(currRoom);
            else if (alpha < m_normalRoomRatio + m_treasureRoomRatio)
                fillTreasureRoom(currRoom);
            else
                fillEliteRoom(currRoom);
        }
    }
    return rooms;
}



std::vector<RGWB::Container> RGWB::generateCorridors(const std::shared_ptr<Tree>& tree) const
{
    auto paths = tree->getPaths();
    std::vector<Container> result;
    result.reserve(paths.size());
    for (auto [from, to] : paths)
    {
        static std::normal_distribution<> d{m_meanCorridorWidth, m_sigmaCorridorWidth};
        int alpha = std::max(1., d(m_gen));
        if (from.x == to.x)
        {
            int64_t min_y = std::min(from.y, to.y);
            int64_t max_y = std::max(from.y, to.y);
            result.emplace_back(Vec2Int{from.x - alpha/2, min_y}, alpha, max_y - min_y);
        }
        else
        {
            int64_t min_x = std::min(from.x, to.x);
            int64_t max_x = std::max(from.x, to.x);
            result.emplace_back(Vec2Int{min_x, from.y - alpha/2}, max_x - min_x, alpha);
        }
    }
    return result;
}



void RGWB::drawGround(TilemapLayer* layer, const std::vector<Container>& containers) const
{
    for (const Container& cont : containers)
    {
        for (int64_t i = 0; i < cont.m_width; ++i)
            for (int64_t j = 0; j < cont.m_height; ++j)
            {
                double alpha = cocos2d::random(0., 1.);
                if (alpha < m_normalGroundRatio)
                    layer->setTileGID(genFromVec(m_config->getGround()), cocos2d::Vec2(i + cont.m_pos.x, j + cont.m_pos.y));
                else
                    layer->setTileGID(genFromVec(m_config->getRareGround()), cocos2d::Vec2(i + cont.m_pos.x, j + cont.m_pos.y));
            }
    }
}



void RGWB::drawBackground(TilemapLayer* layer) const
{
    for (int64_t i = 0; i < m_width; ++i)
        for (int64_t j = 0; j < m_height; ++j)
        {
            double alpha = cocos2d::random(0., 1.);
            if (alpha < m_normalGroundRatio)
                layer->setTileGID(genFromVec(m_config->getWalls()), cocos2d::Vec2(i, j));
            else
                layer->setTileGID(genFromVec(m_config->getRareWalls()), cocos2d::Vec2(i, j));
        }
}



void RGWB::drawCorridors(TilemapLayer* layer, const std::shared_ptr<Tree>& tree) const
{
    auto paths = tree->getPaths();
    for (auto [from, to] : paths)
    {
        int alpha = cocos2d::random(1, 4);
        if (from.x == to.x)
        {
            int64_t bound = std::abs(from.y - to.y);
            for (int64_t i = 0; i < bound; ++i)
                for (int j = -alpha/2; j <= alpha/2; ++j)
                    layer->setTileGID(882 + cocos2d::random(0, 3), cocos2d::Vec2(from.x + j, std::min(from.y, to.y) + i));
        }
        else
        {
            int64_t bound = std::abs(from.x - to.x);
            for (int64_t i = 0; i < bound; ++i)
                for (int j = -alpha/2; j <= alpha/2; ++j)
                    layer->setTileGID(882 + cocos2d::random(0, 3), cocos2d::Vec2(std::min(from.x, to.x) + i, from.y + j));
        }
    }
}



void RGWB::drawRooms(TilemapLayer* layer, const std::shared_ptr<Tree>& tree) const
{
    std::list<Container> leafs = tree->getLeafs();
    for (const Container& cont : leafs)
    {
        Room room(cont);
        for (int64_t i = 0; i < room.m_cont.m_width; ++i)
            for (int64_t j = 0; j < room.m_cont.m_height; ++j)
                layer->setTileGID(882 + cocos2d::random(0, 3), cocos2d::Vec2(i + room.m_cont.m_pos.x, j + room.m_cont.m_pos.y));
    }
}



size_t RGWB::getFarthestRoom(const std::vector<Room>& rooms, const std::vector<size_t>& base) const
{
    size_t result = 0;
    float minMeanDist = std::numeric_limits<float>::max();
    for (size_t i = 0; i < rooms.size(); ++i)
    {
        float currAccDist = 0;
        for (size_t roomIdx : base)
        {
            currAccDist += rooms[i].m_cont.m_pos.distance(rooms[roomIdx].m_cont.m_pos);
        }

        if (currAccDist / base.size() < minMeanDist)
        {
            result = i;
            minMeanDist = currAccDist;
        }
    }
    return result;
}
