#pragma once

/**
 * Interface for updatable classes
 */
struct IUpdatable
{
    virtual ~IUpdatable() = default;
    virtual void update() = 0;
};
