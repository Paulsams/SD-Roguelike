#pragma once

class IStat
{
public:
    virtual ~IStat() = default;
    virtual float getValue() const = 0;
protected:
    float m_value;
};
