#pragma once
#include <functional>
#include <stack>

template <typename T>
class PoolObjects
{
public:
    PoolObjects(std::function<T*()> createFunc)
        : m_createFunc(createFunc) { }
    
    T* get()
    {
        if (m_stack.empty())
            return m_createFunc();

        T* top = m_stack.top();
        m_stack.pop();
        return top;
    }

    void release(T* obj)
    {
        m_stack.push(obj);
    }
    
private:
    std::stack<T*> m_stack;
    std::function<T*()> m_createFunc;
    std::function<T*()> m_releaseFunc;
};
