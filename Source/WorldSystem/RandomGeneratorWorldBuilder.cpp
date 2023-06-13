#include "WorldSystem/RandomGeneratorWorldBuilder.h"
#include "ItemsSystem/Items.h"
#include "ItemsSystem/Attacks.h"

using RGWB = RandomGeneratorWorldBuilder;

Tilemap* RGWB::build() const
{
    if (m_path.empty()
        || m_height == 0
        || m_width == 0
        || m_iterCount == 0
        || std::pow(2, m_iterCount + 1) > m_width
        || std::pow(2, m_iterCount + 1) > m_height
        || !m_config)
    {
        return nullptr;
    }

    return generateWorld();
}



Tilemap* RGWB::generateWorld() const
{
    Tilemap* tileMap = Tilemap::create(m_path);
    cocos2d::Size size(m_width, m_height);
    tileMap->setMapSize(size);

    Container mainContainer(0, 0, m_width, m_height);
    std::shared_ptr<Tree> tree = splitContainer(mainContainer, m_iterCount);

    uint32_t* groundTiles = (uint32_t*)malloc(m_width * m_height * sizeof(uint32_t));
    std::memset(groundTiles, 0, m_width * m_height * sizeof(uint32_t));

    TilemapLayer* groundLayer = tileMap->getLayer("Ground");
    void* oldGroundTiles = groundLayer->getTiles();
    free(oldGroundTiles);

    groundLayer->setLayerSize(size);
    groundLayer->setTiles(groundTiles);

    uint32_t* wallsTiles = (uint32_t*)malloc(m_width * m_height * sizeof(uint32_t));
    std::memset(wallsTiles, 0, m_width * m_height * sizeof(uint32_t));

    TilemapLayer* wallsLayer = tileMap->getLayer("Walls");
    void* oldWallsTiles = wallsLayer->getTiles();
    free(oldWallsTiles);

    wallsLayer->setLayerSize(size);
    wallsLayer->setTiles(wallsTiles);

    std::vector<Room> rooms = generateRooms(tree);

    std::vector<Container> corridors = generateCorridors(tree);
    std::vector<Container> ground(corridors);
    for (const auto& room : rooms)
        ground.push_back(room.m_cont);

    drawWalls(wallsLayer);
    drawGround(wallsLayer, groundLayer, ground);

    cocos2d::TMXObjectGroup* utilsGroup = tileMap->getObjectGroup("UtilsObjects");
    cocos2d::ValueMap spawnPoint = utilsGroup->getObject("SpawnPoint");

    std::vector<cocos2d::Value> decorations;
    std::vector<cocos2d::Value> normalMobs;
    std::vector<cocos2d::Value> eliteMobs;
    std::vector<cocos2d::Value> bossMobs;
    std::vector<cocos2d::Value> passiveMobs;
    std::vector<cocos2d::Value> chests;

    const std::vector<std::string> consumables = Items::getConsumables();
    const std::map<int, std::vector<std::string>> tiers = Attacks::getTiers();
    int maxTier = tiers.rbegin()->first;
    int minTier = tiers.begin()->first;

    for (const Room& room : rooms)
    {
        if (room.m_type == RoomType::SPAWN)
        {
            spawnPoint["x"] = room.m_cont.m_center.x * tileMap->getTileSize().width;
            spawnPoint["y"] = room.m_cont.m_center.y * tileMap->getTileSize().height;
        }

        for (const auto& pos : room.m_decorations)
        {
            cocos2d::ValueMap currDecoration;
            currDecoration["name"] = "Decoration";
            currDecoration["gid"] = genFromVec(m_config->getDecorations());
            currDecoration["x"] = (room.m_cont.m_pos.x + pos.x - 1) * tileMap->getTileSize().width;
            currDecoration["y"] = (room.m_cont.m_pos.y + pos.y - 1) * tileMap->getTileSize().height;
            currDecoration["width"] = tileMap->getTileSize().width;
            currDecoration["height"] = tileMap->getTileSize().height;
            if (cocos2d::random(0., 1.) >= 0.5)
                currDecoration["loot"] = genFromVec(consumables);
            decorations.push_back({cocos2d::Value(currDecoration)});
        }

        for (const auto& pos : room.m_normalMobs)
        {
            cocos2d::ValueMap currNormalMob;
            currNormalMob["name"] = "NormalMobs";
//            currNormalMob["gid"] = genFromVec(m_config->getNormalMobs());
            currNormalMob["gid"] = 176;
            currNormalMob["x"] = (room.m_cont.m_pos.x + pos.x - 1) * tileMap->getTileSize().width;
            currNormalMob["y"] = (room.m_cont.m_pos.y + pos.y - 1) * tileMap->getTileSize().height;
            currNormalMob["width"] = tileMap->getTileSize().width;
            currNormalMob["height"] = tileMap->getTileSize().height;
            normalMobs.push_back({cocos2d::Value(currNormalMob)});
        }

        for (const auto& pos : room.m_eliteMobs)
        {
            cocos2d::ValueMap currEliteMob;
            currEliteMob["name"] = "EliteMobs";
//            currNormalMob["gid"] = genFromVec(m_config->getNormalMobs());
            currEliteMob["gid"] = 375;
            currEliteMob["x"] = (room.m_cont.m_pos.x + pos.x - 1) * tileMap->getTileSize().width;
            currEliteMob["y"] = (room.m_cont.m_pos.y + pos.y - 1) * tileMap->getTileSize().height;
            currEliteMob["width"] = tileMap->getTileSize().width;
            currEliteMob["height"] = tileMap->getTileSize().height;
            eliteMobs.push_back({cocos2d::Value(currEliteMob)});
        }

        for (const auto& pos : room.m_bossMobs)
        {
            cocos2d::ValueMap currBossMob;
            currBossMob["name"] = "BossMobs";
//            currNormalMob["gid"] = genFromVec(m_config->getNormalMobs());
            currBossMob["gid"] = 129;
            currBossMob["x"] = (room.m_cont.m_pos.x + pos.x - 1) * tileMap->getTileSize().width;
            currBossMob["y"] = (room.m_cont.m_pos.y + pos.y - 1) * tileMap->getTileSize().height;
            currBossMob["width"] = tileMap->getTileSize().width;
            currBossMob["height"] = tileMap->getTileSize().height;
            bossMobs.push_back({cocos2d::Value(currBossMob)});
        }

        for (const auto& pos : room.m_passiveMobs)
        {
            cocos2d::ValueMap currPassiveMob;
            currPassiveMob["name"] = "PassiveMobs";
//            currNormalMob["gid"] = genFromVec(m_config->getNormalMobs());
            currPassiveMob["gid"] = 228;
            currPassiveMob["x"] = (room.m_cont.m_pos.x + pos.x - 1) * tileMap->getTileSize().width;
            currPassiveMob["y"] = (room.m_cont.m_pos.y + pos.y - 1) * tileMap->getTileSize().height;
            currPassiveMob["width"] = tileMap->getTileSize().width;
            currPassiveMob["height"] = tileMap->getTileSize().height;
            passiveMobs.push_back({cocos2d::Value(currPassiveMob)});
        }

        for (const auto& pos : room.m_chests)
        {
            int tier = cocos2d::random(minTier, maxTier);
            while (!tiers.contains(tier))
                tier = cocos2d::random(minTier, maxTier);

            cocos2d::ValueMap currChest;
            currChest["name"] = "Chest";
            if (minTier == maxTier)
                currChest["gid"] = m_config->getChests().back();
            else
                currChest["gid"] = m_config->getChests().at((tier - minTier) / (maxTier - minTier) * (m_config->getChests().size() - 1));
            currChest["x"] = (room.m_cont.m_pos.x + pos.x - 1) * tileMap->getTileSize().width;
            currChest["y"] = (room.m_cont.m_pos.y + pos.y - 1) * tileMap->getTileSize().height;
            currChest["width"] = tileMap->getTileSize().width;
            currChest["height"] = tileMap->getTileSize().height;
            currChest["loot"] = genFromVec(tiers.at(tier)) + ':' + std::to_string(tier);
            chests.push_back({cocos2d::Value(currChest)});
        }

    }

    cocos2d::Vector<cocos2d::TMXObjectGroup*> newObjectGroups;
    cocos2d::TMXObjectGroup* utilsObjects = new cocos2d::TMXObjectGroup();
    cocos2d::TMXObjectGroup* decorationsObjects = new cocos2d::TMXObjectGroup();
    cocos2d::TMXObjectGroup* normalMobsObjects = new cocos2d::TMXObjectGroup();
    cocos2d::TMXObjectGroup* eliteMobsObjects = new cocos2d::TMXObjectGroup();
    cocos2d::TMXObjectGroup* bossMobsObjects = new cocos2d::TMXObjectGroup();
    cocos2d::TMXObjectGroup* passiveMobsObjects = new cocos2d::TMXObjectGroup();
    cocos2d::TMXObjectGroup* chestsObjects = new cocos2d::TMXObjectGroup();

    utilsObjects->setGroupName("UtilsObjects");
    utilsObjects->setObjects({cocos2d::Value(spawnPoint)});

    decorationsObjects->setGroupName("Decorations");
    decorationsObjects->setObjects(decorations);

    normalMobsObjects->setGroupName("NormalMobs");
    normalMobsObjects->setObjects(normalMobs);

    eliteMobsObjects->setGroupName("EliteMobs");
    eliteMobsObjects->setObjects(eliteMobs);

    bossMobsObjects->setGroupName("BossMobs");
    bossMobsObjects->setObjects(bossMobs);

    passiveMobsObjects->setGroupName("PassiveMobs");
    passiveMobsObjects->setObjects(passiveMobs);

    chestsObjects->setGroupName("Chests");
    chestsObjects->setObjects(chests);

    newObjectGroups.pushBack(utilsObjects);
    newObjectGroups.pushBack(decorationsObjects);
    newObjectGroups.pushBack(normalMobsObjects);
    newObjectGroups.pushBack(eliteMobsObjects);
    newObjectGroups.pushBack(bossMobsObjects);
    newObjectGroups.pushBack(passiveMobsObjects);
    newObjectGroups.pushBack(chestsObjects);
    tileMap->setObjectGroups(newObjectGroups);

    return tileMap;
}



RGWB::Container::Container(Vec2Int pos, int w, int h)
        : m_pos(pos)
        , m_center(m_pos.x + w/2, m_pos.y + h/2)
        , m_width(w)
        , m_height(h)
{}



RGWB::Container::Container(int x, int y, int w, int h)
        : m_pos(x, y)
        , m_center(m_pos.x + w/2, m_pos.y + h/2)
        , m_width(w)
        , m_height(h)
{}



RGWB::Room::Room(const Container& cont)
{
    int x = cont.m_pos.x + cocos2d::random(1, std::max(2, cont.m_width/s_partDiv));
    int y = cont.m_pos.y + cocos2d::random(1, std::max(2, cont.m_height/s_partDiv));
    int width = cont.m_width - (x - cont.m_pos.x);
    int height = cont.m_height - (y - cont.m_pos.y);

    int diffWidth = cocos2d::random(1, std::max(2, width/s_partDiv));
    int diffHeight = cocos2d::random(1, std::max(2, height/s_partDiv));

    if (width - diffWidth > 2)
        width -= diffWidth;
    else
        --width;

    if (height - diffHeight > 2)
        height -= diffHeight;
    else
        --height;

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
        c1 = Container{cont.m_pos, cocos2d::random(2, cont.m_width), cont.m_height};
        c2 = Container{cont.m_pos.x + c1.m_width, cont.m_pos.y, cont.m_width - c1.m_width, cont.m_height};

        double c1WR = static_cast<double>(c1.m_width) / c1.m_height;
        double c2WR = static_cast<double>(c2.m_width) / c2.m_height;
        if (c1WR < m_widthRatio || c2WR < m_widthRatio)
            return randomSplit(cont);
    }
    else
    {
        // Horizontal
        c1 = Container{cont.m_pos, cont.m_width, cocos2d::random(2, cont.m_height)};
        c2 = Container{cont.m_pos.x, cont.m_pos.y + c1.m_height, cont.m_width, cont.m_height - c1.m_height};

        double c1HR = static_cast<double>(c1.m_height) / c1.m_width;
        double c2HR = static_cast<double>(c2.m_height) / c2.m_width;
        if (c1HR < m_heightRatio || c2HR < m_heightRatio)
            return randomSplit(cont);
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



void RGWB::fillRoomVec2int(Room& room, std::vector<Vec2Int>& mobs, int counter, int tryCounter) const
{
    for (int i = 0; i < counter; ++i)
    {
        Vec2Int pos;
        do {
            pos.x = cocos2d::random(0, room.m_cont.m_width - 1) + 1;
            pos.y = cocos2d::random(0, room.m_cont.m_height - 1);
            --tryCounter;
        } while (room.m_engaged.contains(pos) && tryCounter);
        mobs.push_back(pos);
        room.m_engaged.insert(pos);
    }
}



void RGWB::fillSpawnRoom(Room& room) const
{
    room.m_type = RoomType::SPAWN;

    room.m_decorations.emplace_back(Vec2Int{1,0});
    room.m_decorations.emplace_back(Vec2Int{1, room.m_cont.m_height - 1});
    room.m_decorations.emplace_back(Vec2Int{room.m_cont.m_width, 0});
    room.m_decorations.emplace_back(Vec2Int{room.m_cont.m_width, room.m_cont.m_height - 1});

    room.m_engaged.insert(Vec2Int{room.m_cont.m_width / 2, room.m_cont.m_height / 2});
    room.m_engaged.insert(Vec2Int{1,0});
    room.m_engaged.insert(Vec2Int{1, room.m_cont.m_height - 1});
    room.m_engaged.insert(Vec2Int{room.m_cont.m_width, 0});
    room.m_engaged.insert(Vec2Int{room.m_cont.m_width, room.m_cont.m_height - 1});
}



void RGWB::fillSingleBossRoom(Room& room) const
{
    static std::exponential_distribution<> d(1/m_bossRoomTreasureMean);
    room.m_type = RoomType::BOSS;
    room.m_bossMobs.emplace_back(room.m_cont.m_width / 2, room.m_cont.m_height / 2);
    room.m_engaged.insert(Vec2Int{room.m_cont.m_width / 2, room.m_cont.m_height / 2});

    int roomArea = room.m_cont.m_width * room.m_cont.m_height;
    int tryCounter = m_width * m_height;

    int normalMobsCount = roomArea * m_minRoomFillBound;
    fillRoomVec2int(room, room.m_normalMobs, normalMobsCount, tryCounter);

    int decorationsCount = roomArea * m_minRoomFillBound;
    fillRoomVec2int(room, room.m_decorations, decorationsCount, tryCounter);

    int chestsCount = d(m_gen);
    fillRoomVec2int(room, room.m_chests, chestsCount, tryCounter);
}



void RGWB::fillNormalRoom(Room& room) const
{
    static std::exponential_distribution<> d(1/m_normalRoomTreasureMean);
    room.m_type = RoomType::NORMAL;
    int roomArea = room.m_cont.m_width * room.m_cont.m_height;
    int tryCounter = m_width * m_height;

    int passiveMobsCount = roomArea * m_minRoomFillBound;
    fillRoomVec2int(room, room.m_passiveMobs, passiveMobsCount, tryCounter);

    int normalMobsCount = roomArea * cocos2d::random(m_minRoomFillBound, m_maxRoomFillBound);
    fillRoomVec2int(room, room.m_normalMobs, normalMobsCount, tryCounter);

    int decorationsCount = roomArea * m_minRoomFillBound;
    fillRoomVec2int(room, room.m_decorations, decorationsCount, tryCounter);

    int chestsCount = d(m_gen);
    fillRoomVec2int(room, room.m_chests, chestsCount, tryCounter);
}



void RGWB::fillEliteRoom(Room& room) const
{
    static std::exponential_distribution<> d(1/m_eliteRoomTreasureMean);
    room.m_type = RoomType::ELITE;
    int roomArea = room.m_cont.m_width * room.m_cont.m_height;
    int tryCounter = m_width * m_height;

    int passiveMobsCount = roomArea * m_minRoomFillBound;
    fillRoomVec2int(room, room.m_passiveMobs, passiveMobsCount, tryCounter);

    int eliteMobsCount = roomArea * m_minRoomFillBound;
    fillRoomVec2int(room, room.m_eliteMobs, eliteMobsCount, tryCounter);

    int normalMobsCount = roomArea * cocos2d::random(m_minRoomFillBound, m_maxRoomFillBound);
    fillRoomVec2int(room, room.m_normalMobs, normalMobsCount, tryCounter);

    int decorationsCount = roomArea * m_minRoomFillBound;
    fillRoomVec2int(room, room.m_decorations, decorationsCount, tryCounter);

    int chestsCount = d(m_gen);
    fillRoomVec2int(room, room.m_chests, chestsCount, tryCounter);
}



void RGWB::fillTreasureRoom(Room& room) const
{
    static std::exponential_distribution<> d(1/m_treasureRoomTreasureMean);
    room.m_type = RoomType::TREASURE;
    int roomArea = room.m_cont.m_width * room.m_cont.m_height;
    int tryCounter = m_width * m_height;

    int passiveMobsCount = roomArea * m_minRoomFillBound;
    fillRoomVec2int(room, room.m_passiveMobs, passiveMobsCount, tryCounter);

    int decorationsCount = roomArea * m_maxRoomFillBound;
    fillRoomVec2int(room, room.m_decorations, decorationsCount, tryCounter);

    int chestsCount = d(m_gen);
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
    int bossCount = m_iterCount;
    for (int i = 0; i < bossCount; ++i)
    {
        size_t idx = cocos2d::random((size_t)0, rooms.size() - 1);
        while (rooms[idx].m_type != RoomType::NONE)
        {
            idx = cocos2d::random((size_t)0, rooms.size() - 1);
        }
        fillSingleBossRoom(rooms[idx]);
    }

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
            int minY = std::min(from.y, to.y);
            int maxY = std::max(from.y, to.y);
            result.emplace_back(Vec2Int{from.x - alpha/2, minY}, alpha, maxY - minY);
        }
        else
        {
            int minX = std::min(from.x, to.x);
            int maxX = std::max(from.x, to.x);
            result.emplace_back(Vec2Int{minX, from.y - alpha/2}, maxX - minX, alpha);
        }
    }
    return result;
}



void RGWB::drawWalls(TilemapLayer* layer) const
{
    for (int i = 0; i < m_width; ++i)
    {
        for (int j = 0; j < m_height; ++j)
        {
            double alpha = cocos2d::random(0., 1.);

            if (alpha < m_normalTileRatio)
                layer->setTileGID(genFromVec(m_config->getWalls()), cocos2d::Vec2(i, j));
            else
                layer->setTileGID(genFromVec(m_config->getRareWalls()), cocos2d::Vec2(i, j));
        }
    }
}



void RGWB::drawGround(TilemapLayer* wallsLayer, TilemapLayer* groundLayer, const std::vector<Container>& ground) const
{
    for (const Container& cont : ground)
    {
        for (int i = 0; i < cont.m_width; ++i)
        {
            for (int j = 0; j < cont.m_height; ++j)
            {
                double alpha = cocos2d::random(0., 1.);
                if (alpha < m_normalTileRatio)
                    groundLayer->setTileGID(genFromVec(m_config->getGround()), cocos2d::Vec2(i + cont.m_pos.x, m_height - (1 + j + cont.m_pos.y)));
                else
                    groundLayer->setTileGID(genFromVec(m_config->getRareGround()), cocos2d::Vec2(i + cont.m_pos.x, m_height - (1 + j + cont.m_pos.y)));

                wallsLayer->setTileGID(0, cocos2d::Vec2(i + cont.m_pos.x, m_height - (1 + j + cont.m_pos.y)));
            }
        }
    }
}
