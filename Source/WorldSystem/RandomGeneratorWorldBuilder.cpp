#include "WorldSystem/RandomGeneratorWorldBuilder.h"
#include "ItemsSystem/Items.h"
#include "ItemsSystem/Attacks.h"

#include <ranges>

using RGWB = RandomGeneratorWorldBuilder;

std::random_device RGWB::s_rd{};
std::mt19937 RGWB::s_gen(s_rd());

cocos2d::TMXMapInfo* RGWB::build() const
{
    if (m_path.empty()
     || std::pow(2, m_iterCount) > m_width * (1 - 1.0/Room::s_partDiv)
     || std::pow(2, m_iterCount) > m_height * (1 - 1.0/Room::s_partDiv)
     || !m_config
    )
    {
        return nullptr;
    }

    return generateWorld();
}



void RGWB::fillObjectMetaInfo(cocos2d::ValueMap& object, cocos2d::TMXMapInfo* mapInfo, const Room& room, Vec2Int pos)
{
    object["x"] = (room.box.pos.x + pos.x - 1.0) * mapInfo->getTileSize().width;
    object["y"] = (room.box.pos.y + pos.y - 1.0) * mapInfo->getTileSize().height;
    object["width"] = mapInfo->getTileSize().width;
    object["height"] = mapInfo->getTileSize().height;
}



cocos2d::TMXMapInfo* RGWB::generateWorld() const
{
    cocos2d::TMXMapInfo* mapInfo = cocos2d::TMXMapInfo::create(m_path);

    cocos2d::Size size(m_width, m_height);
    mapInfo->setMapSize(size);

    Box mainBox(0, 0, m_width, m_height);
    std::shared_ptr<Tree> tree = splitContainer(mainBox, m_iterCount);

    cocos2d::Vector<cocos2d::TMXLayerInfo*>& layers = mapInfo->getLayers();
    cocos2d::TMXLayerInfo* groundLayer;
    cocos2d::TMXLayerInfo* wallsLayer;

    for (cocos2d::TMXLayerInfo* info : layers)
    {
        if (info->_name == "Ground")
            groundLayer = info;
        else if (info->_name == "Walls")
            wallsLayer = info;
    }

    uint32_t* groundTiles = (uint32_t*)std::malloc(m_width * m_height * sizeof(uint32_t));
    std::memset(groundTiles, 0, m_width * m_height * sizeof(uint32_t));

    free(groundLayer->_tiles);
    groundLayer->_layerSize = size;
    groundLayer->_tiles = groundTiles;

    uint32_t* wallsTiles = (uint32_t*)std::malloc(m_width * m_height * sizeof(uint32_t));
    std::memset(wallsTiles, 0, m_width * m_height * sizeof(uint32_t));

    free(wallsLayer->_tiles);
    wallsLayer->_layerSize = size;
    wallsLayer->_tiles = wallsTiles;

    std::vector<Room> rooms = generateRooms(tree);

    std::vector<Box> corridors = generateCorridors(tree);
    std::vector<Box> ground(corridors);
    std::ranges::transform(rooms, std::back_inserter(ground), [](const auto& r) { return r.box; } );

    drawWalls(wallsLayer);
    drawGround(wallsLayer, groundLayer, ground);

    std::vector<cocos2d::Value> decorations;
    std::vector<cocos2d::Value> normalMobs;
    std::vector<cocos2d::Value> eliteMobs;
    std::vector<cocos2d::Value> bossMobs;
    std::vector<cocos2d::Value> passiveMobs;
    std::vector<cocos2d::Value> chests;
    std::vector<cocos2d::Value> utils;

    const std::vector<std::string> consumables = Items::getConsumables();
    const std::vector<std::pair<std::string, int>> weapons = Attacks::getTiers();
    auto [minIt, maxIt] = std::ranges::minmax_element(weapons, std::less{}, &std::pair<std::string, int>::second);
    int minTier = minIt->second;
    int maxTier = maxIt->second;

    for (const Room& room : rooms)
    {
        if (room.type == RoomType::SPAWN)
        {
            cocos2d::ValueMap spawnPoint;
            spawnPoint["name"] = "SpawnPoint";
            fillObjectMetaInfo(spawnPoint, mapInfo, room, Vec2Int{room.box.center.x - room.box.pos.x + 1, room.box.center.y - room.box.pos.y + 1});
            utils.emplace_back(spawnPoint);
        }

        for (Vec2Int pos : room.decorations)
        {
            cocos2d::ValueMap currDecoration;
            currDecoration["name"] = "Decoration";
            currDecoration["gid"] = genFromVec(m_config->getDecorations());
            fillObjectMetaInfo(currDecoration, mapInfo, room, pos);
            if (cocos2d::random(0.0, 1.0) >= 0.5)
                currDecoration["loot"] = genFromVec(consumables);
            decorations.emplace_back(currDecoration);
        }

        for (Vec2Int pos : room.normalMobs)
        {
            cocos2d::ValueMap currNormalMob;
            currNormalMob["name"] = "NormalMobs";
            currNormalMob["gid"] = genFromVec(m_config->getNormalMobs());
            fillObjectMetaInfo(currNormalMob, mapInfo, room, pos);
            normalMobs.emplace_back(currNormalMob);
        }

        for (Vec2Int pos : room.eliteMobs)
        {
            cocos2d::ValueMap currEliteMob;
            currEliteMob["name"] = "EliteMobs";
            currEliteMob["gid"] = genFromVec(m_config->getEliteMobs());
            fillObjectMetaInfo(currEliteMob, mapInfo, room, pos);
            eliteMobs.emplace_back(currEliteMob);
        }

        for (Vec2Int pos : room.bossMobs)
        {
            cocos2d::ValueMap currBossMob;
            currBossMob["name"] = "BossMobs";
            currBossMob["gid"] = genFromVec(m_config->getBossMobs());
            fillObjectMetaInfo(currBossMob, mapInfo, room, pos);
            bossMobs.emplace_back(currBossMob);
        }

        for (Vec2Int pos : room.passiveMobs)
        {
            cocos2d::ValueMap currPassiveMob;
            currPassiveMob["name"] = "PassiveMobs";
            currPassiveMob["gid"] = genFromVec(m_config->getPassiveMobs());
            fillObjectMetaInfo(currPassiveMob, mapInfo, room, pos);
            passiveMobs.emplace_back(currPassiveMob);
        }

        for (Vec2Int pos : room.chests)
        {
            cocos2d::ValueMap currChest;
            auto [weaponName, weaponTier] = genFromVec(weapons);
            currChest["name"] = "Chest";
            if (minTier == maxTier)
                currChest["gid"] = m_config->getChests().back();
            else
                currChest["gid"] = m_config->getChests().at((m_config->getChests().size() - 1.0) * (weaponTier - minTier) / (maxTier - minTier));
            currChest["loot"] = weaponName + ':' + std::to_string(weaponTier);
            fillObjectMetaInfo(currChest, mapInfo, room, pos);
            chests.emplace_back(currChest);
        }
    }

    for (cocos2d::TMXObjectGroup* group : mapInfo->getObjectGroups())
    {
        const std::string& name = group->getGroupName();
        if (name == "UtilsObjects")
            group->setObjects(utils);
        else if (name == "Decorations")
            group->setObjects(decorations);
        else if (name == "NormalMobs")
            group->setObjects(normalMobs);
        else if (name == "EliteMobs")
            group->setObjects(eliteMobs);
        else if (name == "BossMobs")
            group->setObjects(bossMobs);
        else if (name == "PassiveMobs")
            group->setObjects(passiveMobs);
        else if (name == "Chests")
            group->setObjects(chests);
    }

    return mapInfo;
}



RGWB::Box::Box(Vec2Int pos, int w, int h)
        : pos(pos)
        , center(pos.x + w/2, pos.y + h/2)
        , width(w)
        , height(h)
{}



RGWB::Box::Box(int x, int y, int w, int h)
        : pos(x, y)
        , center(pos.x + w/2, pos.y + h/2)
        , width(w)
        , height(h)
{}



RGWB::Room::Room(const Box& b)
{
    int x = b.pos.x + cocos2d::random(1, std::max(2, b.width/s_partDiv));
    int y = b.pos.y + cocos2d::random(1, std::max(2, b.height/s_partDiv));

    int width = b.width - (x - b.pos.x);
    int height = b.height - (y - b.pos.y);

    int diffWidth = cocos2d::random(1, std::max(2, width/s_partDiv));
    int diffHeight = cocos2d::random(1, std::max(2, height/s_partDiv));

    width = std::max(width - diffWidth, 2);
    height = std::max(height - diffHeight, 2);

    box = Box{x, y, width, height};
}



std::list<RGWB::Box> RGWB::Tree::getLeafs() const
{
    if (!left && !right)
        return {leaf};
    std::list<Box> res = left->getLeafs();
    res.splice(res.end(), right->getLeafs());
    return res;
}



std::list<std::pair<Vec2Int, Vec2Int>> RGWB::Tree::getPaths() const
{
    if (!left && !right)
        return {};
    std::list<std::pair<Vec2Int, Vec2Int>> res = {std::make_pair(left->leaf.center, right->leaf.center)};
    res.splice(res.end(), left->getPaths());
    res.splice(res.end(), right->getPaths());
    return res;
}



std::pair<RGWB::Box, RGWB::Box> RGWB::randomSplit(const Box& box) const
{
    Box b1;
    Box b2;
    if (cocos2d::random() % 2 == 0)
    {
        // Vertical
        b1 = Box{box.pos, cocos2d::random(1, box.width), box.height};
        b2 = Box{box.pos.x + b1.width, box.pos.y, box.width - b1.width, box.height};

        double b1WR = static_cast<double>(b1.width) / b1.height;
        double b2WR = static_cast<double>(b2.width) / b2.height;
        if (b1.width * b1.height == 0 || b2.width * b2.height == 0 || b1WR < m_widthRatio || b2WR < m_widthRatio)
            return randomSplit(box);
    }
    else
    {
        // Horizontal
        b1 = Box{box.pos, box.width, cocos2d::random(1, box.height)};
        b2 = Box{box.pos.x, box.pos.y + b1.height, box.width, box.height - b1.height};

        double b1HR = static_cast<double>(b1.height) / b1.width;
        double b2HR = static_cast<double>(b2.height) / b2.width;
        if (b1.width * b1.height == 0 || b2.width * b2.height == 0 || b1HR < m_heightRatio || b2HR < m_heightRatio)
            return randomSplit(box);
    }
    return {b1, b2};
}



std::shared_ptr<RGWB::Tree> RGWB::splitContainer(const Box& box, size_t iterCount) const
{
    auto root = std::make_shared<Tree>(box);
    if (iterCount > 0)
    {
        auto [b1, b2] = randomSplit(box);
        root->left = splitContainer(b1, iterCount - 1);
        root->right = splitContainer(b2, iterCount - 1);
    }
    return root;
}



void RGWB::Room::fillEntities(std::vector<Vec2Int>& mobs, int amount)
{
    int counter = box.width * box.height;
    for (int i = 0; i < amount; ++i)
    {
        Vec2Int pos;
        do {
            pos.x = cocos2d::random(0, box.width - 1) + 1; // cocos2d TileMap API :(
            pos.y = cocos2d::random(0, box.height - 1);
            --counter;
        } while (engaged.contains(pos) && counter > 0);
        mobs.push_back(pos);
        engaged.insert(pos);
    }
}



void RGWB::Room::createSpawnEnv()
{
    type = RoomType::SPAWN;
}



void RGWB::Room::createBossEnv()
{
    static std::exponential_distribution<> d(1/s_bossRoomTreasureMean);
    type = RoomType::BOSS;
    bossMobs.emplace_back(box.width / 2, box.height / 2);
    engaged.emplace(box.width / 2, box.height / 2);
    fillEntities(normalMobs, box.getArea() * s_minRoomFillBound);
    fillEntities(decorations, box.getArea() * cocos2d::random(s_minRoomFillBound, s_maxRoomFillBound));
    fillEntities(chests, d(s_gen));
}



void RGWB::Room::createNormalEnv()
{
    static std::exponential_distribution<> d(1/s_normalRoomTreasureMean);
    type = RoomType::NORMAL;
    fillEntities(passiveMobs, box.getArea() * cocos2d::random(s_minRoomFillBound, s_maxRoomFillBound));
    fillEntities(normalMobs, box.getArea() * cocos2d::random(s_minRoomFillBound, s_maxRoomFillBound));
    fillEntities(decorations, box.getArea() * cocos2d::random(s_minRoomFillBound, s_maxRoomFillBound));
    fillEntities(chests, d(s_gen));
}



void RGWB::Room::createEliteEnv()
{
    static std::exponential_distribution<> d(1/s_eliteRoomTreasureMean);
    type = RoomType::ELITE;
    fillEntities(passiveMobs, box.getArea() * s_minRoomFillBound);
    fillEntities(eliteMobs, box.getArea() * s_minRoomFillBound);
    fillEntities(normalMobs, box.getArea() * cocos2d::random(s_minRoomFillBound, s_maxRoomFillBound));
    fillEntities(decorations, box.getArea() * cocos2d::random(s_minRoomFillBound, s_maxRoomFillBound));
    fillEntities(chests, d(s_gen));
}



void RGWB::Room::createTreasureEnv()
{
    static std::exponential_distribution<> d(1/s_treasureRoomTreasureMean);
    type = RoomType::TREASURE;
    fillEntities(passiveMobs, box.getArea() * s_maxRoomFillBound);
    fillEntities(decorations, box.getArea() * s_maxRoomFillBound);
    fillEntities(chests, d(s_gen));
}



std::vector<RGWB::Room> RGWB::generateRooms(const std::shared_ptr<Tree>& tree) const
{
    std::list<Box> leafs = tree->getLeafs();
    std::vector<Room> rooms;
    rooms.reserve(leafs.size());

    size_t spawnRoomIdx = 0;
    size_t currIdx = 0;
    size_t smallestArea = -1;
    for (const Box& box : leafs)
    {
        rooms.emplace_back(box);
        if (smallestArea > rooms.back().box.width * rooms.back().box.height && rooms.back().box.width * rooms.back().box.height > 0)
        {
            spawnRoomIdx = currIdx;
            smallestArea = rooms.back().box.width * rooms.back().box.height;
        }
        ++currIdx;
    }

    rooms[spawnRoomIdx].createSpawnEnv();
    int bossCount = m_iterCount;
    for (int i = 0; i < bossCount; ++i)
    {
        size_t idx = cocos2d::random((size_t)0, rooms.size() - 1);
        while (rooms[idx].type != RoomType::NONE)
            idx = cocos2d::random((size_t)0, rooms.size() - 1);

        rooms[idx].createBossEnv();
    }

    for (Room& currRoom : rooms)
    {
        if (currRoom.type == RoomType::NONE)
        {
            double alpha = cocos2d::random(0.0, 1.0);
            if (alpha < m_normalRoomRatio)
                currRoom.createNormalEnv();
            else if (alpha < m_normalRoomRatio + m_treasureRoomRatio)
                currRoom.createTreasureEnv();
            else
                currRoom.createEliteEnv();
        }
    }
    return rooms;
}



std::vector<RGWB::Box> RGWB::generateCorridors(const std::shared_ptr<Tree>& tree) const
{
    auto paths = tree->getPaths();
    std::vector<Box> result;
    result.reserve(paths.size());
    for (const auto [from, to] : paths)
    {
        static std::normal_distribution<> d{s_meanCorridorWidth, s_sigmaCorridorWidth};
        int alpha = static_cast<int>(std::round(std::max(1.0, d(s_gen))));
        if (from.x == to.x)
        {
            auto [minY, maxY] = std::minmax(from.y, to.y);
            result.emplace_back(Vec2Int{from.x - alpha/2, minY}, alpha, maxY - minY);
        }
        else
        {
            auto [minX, maxX] = std::minmax(from.x, to.x);
            result.emplace_back(Vec2Int{minX, from.y - alpha/2}, maxX - minX, alpha);
        }
    }
    return result;
}



void RGWB::drawWalls(LayerInfo* layer) const
{
    for (int i = 0; i < m_width; ++i)
    {
        for (int j = 0; j < m_height; ++j)
        {
            if (cocos2d::random(0., 1.) < m_normalTileRatio)
                layer->_tiles[i + m_width * j] = genFromVec(m_config->getWalls());
            else
                layer->_tiles[i + m_width * j] = genFromVec(m_config->getRareWalls());
        }
    }
}



void RGWB::drawGround(LayerInfo* wallsLayer, LayerInfo* groundLayer, const std::vector<Box>& ground) const
{
    for (const Box& box : ground)
    {
        for (int i = 0; i < box.width; ++i)
        {
            for (int j = 0; j < box.height; ++j)
            {
                if (cocos2d::random(0., 1.) < m_normalTileRatio)
                    groundLayer->_tiles[i + box.pos.x + m_width * (m_height - (1 + j + box.pos.y))] = genFromVec(m_config->getGround());
                else
                    groundLayer->_tiles[i + box.pos.x + m_width * (m_height - (1 + j + box.pos.y))] = genFromVec(m_config->getRareGround());

                wallsLayer->_tiles[i + box.pos.x + m_width * (m_height - (1 + j + box.pos.y))] = 0;
            }
        }
    }
}
