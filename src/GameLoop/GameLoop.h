#pragma once
#include <set>

#include "IUpdatable.h"

class GameLoop
{
public:
    void add(IUpdatable* updater);
    void remove(IUpdatable* updater);
    void step();
private:
    std::set<IUpdatable*> m_updaters;
};
