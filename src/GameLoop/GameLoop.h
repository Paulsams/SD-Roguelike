#pragma once
#include <set>

#include "IUpdatable.h"
#include "UI/Canvas.h"
#include "Utils/ValueNotifyChanged.h"

class GameLoop
{
public:
    explicit GameLoop();
    
    void add(IUpdatable* updater);
    void remove(IUpdatable* updater);
    void step();
    
    ValueNotifyChanged<uint32_t> currentStep;
private:
    std::set<IUpdatable*> m_updaters;
};
