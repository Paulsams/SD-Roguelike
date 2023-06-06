#pragma once

#include <functional>
#include <list>

template <typename... Args>
class FunctionHandler
{
public:
	typedef std::function<void(Args...)> handler_func_type;
	typedef unsigned int handler_id_type;

	FunctionHandler(const handler_func_type& handlerFunc)
		: m_handlerFunc(handlerFunc)
	{
		m_handlerId = ++m_handlerIdCounter;
	}

	FunctionHandler(const FunctionHandler& src)
		: m_handlerFunc(src.m_handlerFunc), m_handlerId(src.m_handlerId)
	{
	}

	FunctionHandler(FunctionHandler&& src) noexcept
		: m_handlerFunc(std::move(src.m_handlerFunc)), m_handlerId(src.m_handlerId)
	{
	}

	FunctionHandler& operator=(const FunctionHandler& src)
	{
		m_handlerFunc = src.m_handlerFunc;
		m_handlerId = src.m_handlerId;

		return *this;
	}

	FunctionHandler& operator=(FunctionHandler&& src) noexcept
	{
		std::swap(m_handlerFunc, src.m_handlerFunc);
		m_handlerId = src.m_handlerId;

		return *this;
	}

	void operator()(Args... params) const
	{
		if (m_handlerFunc)
		{
			m_handlerFunc(params...);
		}
	}

	bool operator==(const FunctionHandler& other) const
	{
		return m_handlerId == other.m_handlerId;
	}

	operator bool() const
	{
		return m_handlerFunc;
	}

	handler_id_type id() const
	{
		return m_handlerId;
	}

private:
	handler_func_type m_handlerFunc;
	handler_id_type m_handlerId;
	static uint32_t m_handlerIdCounter;
};

template <typename... Args>
uint32_t FunctionHandler<Args...>::m_handlerIdCounter(0);

template <typename... Args>
class EventContainer
{
public:
	typedef FunctionHandler<Args...> handler_type;

	EventContainer()
	{
	}

	EventContainer(const EventContainer& src)
		: m_handlers(src.m_handlers) { }

	EventContainer(EventContainer&& src) noexcept
		: m_handlers(std::move(src.m_handlers)) { }

	EventContainer& operator=(const EventContainer& src)
	{
		m_handlers = src.m_handlers;
		return *this;
	}

	EventContainer& operator=(EventContainer&& src) noexcept
	{
		std::swap(m_handlers, src.m_handlers);
		return *this;
	}

	typename handler_type::handler_id_type add(const handler_type& handler)
	{
		m_handlers.push_back(handler);
		return handler.id();
	}

	inline typename handler_type::handler_id_type add(const typename handler_type::handler_func_type& handler)
	{
		return add(handler_type(handler));
	}

	bool remove(const handler_type& handler)
	{
		auto it = std::find(m_handlers.begin(), m_handlers.end(), handler);
		if (it != m_handlers.end())
		{
			m_handlers.erase(it);
			return true;
		}

		return false;
	}

	bool remove_id(const typename handler_type::handler_id_type& handlerId)
	{
		auto it = std::find_if(m_handlers.begin(), m_handlers.end(),
			[handlerId](const handler_type& handler) { return handler.id() == handlerId; });
		if (it != m_handlers.end())
		{
			m_handlers.erase(it);
			return true;
		}

		return false;
	}

	inline void invoke(Args... params) const
	{
		for (const auto& handler : m_handlers)
			handler(params...);
	}

	inline void operator()(Args... params) const
	{
		invoke(params...);
	}

	inline typename handler_type::handler_id_type operator+=(const handler_type& handler)
	{
		return add(handler);
	}

	inline typename handler_type::handler_id_type operator+=(const typename handler_type::handler_func_type& handler)
	{
		return add(handler);
	}

	inline bool operator-=(const handler_type& handler)
	{
		return remove(handler);
	}
private:
	typedef std::list<handler_type> handler_collection_type;
	
	handler_collection_type m_handlers;
};

template <typename T>
class ObservableVector
{
public:
	using oldValue = const T&;
	using newValue = const T&;
	
	EventContainer<size_t, newValue> added;
	EventContainer<size_t, newValue> removed;
	EventContainer<size_t, oldValue, newValue> changed;
	EventContainer<size_t, size_t> swapped;

	void push_back(const T& value)
	{
		m_collection.push_back(value);
		added(m_collection.size() - 1, value);
	}

	void push_back(T&& value)
	{
		m_collection.push_back(std::move(value));
		added(m_collection.size() - 1, m_collection[m_collection.size() - 1]);
	}

	void erase(const T& value)
	{
		auto finded = std::ranges::find(m_collection, value);
		m_collection.erase(finded);
		removed(finded - m_collection.begin(), finded);
	}

	void eraseAt(size_t index)
	{
		const T& value = m_collection[index];
		m_collection.erase(m_collection.begin() + index);
		removed(index, value);
	}

	const T& operator[] (size_t index) const
	{
		return m_collection[index];
	}

	void setAt(size_t index, const T& value)
	{
		const T& old = m_collection[index];
		m_collection[index] = value;
		changed(index, old, value);
	}

	void setAt(size_t index, T&& value)
	{
		const T& old = m_collection[index];
		m_collection[index] = std::move(value);
		changed(index, old, value);
	}

	void swap(size_t firstIndex, size_t secondIndex)
	{
		std::swap(m_collection[firstIndex], m_collection[secondIndex]);
	}

	const std::vector<T>& getCollection() const { return m_collection; }
	
private:
	std::vector<T> m_collection;
};
