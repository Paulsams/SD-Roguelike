#pragma once
#include <set>

#include "EventsSystem.h"

template <typename T>
class ValueNotifyChanged
{
public:
    EventContainer<T> changed;

    explicit ValueNotifyChanged(const T& defaultValue)
        : m_value(defaultValue) { }

    ValueNotifyChanged() = default;

    void change(const T& value)
    {
        if (value == m_value)
            return;

        m_value = value;
        changed(value);
    }

    const T& get() const
    {
        return m_value;
    }

private:
    T m_value;
};
