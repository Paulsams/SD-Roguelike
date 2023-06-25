#pragma once
#include <unordered_set>

#include "GameLoop/IUpdatable.h"
#include "Utils/ValueNotifyChanged.h"


/**
 * Represents the main loop of the game
 */
class GameLoop
{
public:
    GameLoop() = default;

    /**
     * Adds new updater to the loop
     * @param updater
     */
    void add(IUpdatable* updater);

    /**
     * Removes updater from the loop
     * @param updater
     */
    void remove(IUpdatable* updater);

    /**
     * Perform single step(turn) in the loop
     * i.e. make one cycle of the game loop
     *
     * Calls update from every updater
     */
    void step();

private:
    std::unordered_set<IUpdatable*> m_updaters;
};
