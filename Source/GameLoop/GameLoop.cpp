#include "GameLoop/GameLoop.h"

void GameLoop::add(IUpdatable* updater)
{
    m_updaters.emplace(updater);
}

void GameLoop::remove(IUpdatable* updater)
{
    m_updaters.erase(updater);
}

void GameLoop::step()
{
    for (IUpdatable* updater : m_updaters)
        updater->update();
}
