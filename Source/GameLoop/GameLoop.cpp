#include "GameLoop/GameLoop.h"

GameLoop::GameLoop()
    : currentStep(0) { }

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
    currentStep.change(currentStep.get() + 1);
    for (IUpdatable* updater : m_updaters)
        updater->update();
}
