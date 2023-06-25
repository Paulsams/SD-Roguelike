#pragma once
#include <functional>
#include <optional>

#include "IVisitorEntities.h"

template<typename ReturnType>
struct FunctionVisitorEntitiesBuilder;

/**
 * Implementation of a visitor for entities on functors that do not return void
 * @tparam ReturnType return type of functors (cannot be void)
 */
template<typename ReturnType>
class FunctionVisitorEntities : public IVisitorEntities
{
    friend struct FunctionVisitorEntitiesBuilder<ReturnType>;
public:
    /**
     * Implementing a visit for a mob
     * @param mob mob
     */
    void visitMob(mob::Mob* mob) override { if (m_visitMob) m_returnValue = m_visitMob(mob); }
    /**
     * Implementing a visit for a player
     * @param player player
     */
    void visitPlayer(Player* player) override { if (m_visitPlayer) m_returnValue = m_visitPlayer(player); }
    /**
     * Implementing a visit for a item
     * @param item item
     */
    void visitItem(BaseItem* item) override { if (m_visitItem) m_returnValue = m_visitItem(item); }
    /**
     * Implementing a visit for a chest
     * @param chest chest
     */
    void visitChest(Chest* chest) override { if (m_visitChest) m_returnValue = m_visitChest(chest); }
    /**
     * Implementing a visit for a decoration
     * @param decoration decoration
     */
    void visitDecoration(Decoration* decoration) override { if (m_visitDecoration) m_returnValue = m_visitDecoration(decoration); }

    /**
     * Returns the value returned by the last called function \n
     * Causes the value to be reset after taking
     * @return if the function was called, it will return optional with some value, otherwise it will return empty
     */
    std::optional<ReturnType> getReturnValue()
    {
        std::optional<ReturnType> copied = m_returnValue;
        m_returnValue.reset();
        return copied;
    }

private:
    FunctionVisitorEntities(
            std::function<ReturnType(mob::Mob*)>&& visitMob,
            std::function<ReturnType(Player*)>&& visitPlayer,
            std::function<ReturnType(BaseItem*)>&& visitItem,
            std::function<ReturnType(Chest*)>&& visitChest,
            std::function<ReturnType(Decoration*)>&& visitDecoration)
        : m_visitMob(std::move(visitMob))
        , m_visitPlayer(std::move(visitPlayer))
        , m_visitItem(std::move(visitItem))
        , m_visitChest(std::move(visitChest))
        , m_visitDecoration(std::move(visitDecoration)) { }
    
    std::function<ReturnType(mob::Mob*)> m_visitMob;
    std::function<ReturnType(Player*)> m_visitPlayer;
    std::function<ReturnType(BaseItem*)> m_visitItem;
    std::function<ReturnType(Chest*)> m_visitChest;
    std::function<ReturnType(Decoration*)> m_visitDecoration;

    std::optional<ReturnType> m_returnValue;
};

/**
 * Implementation of a visitor for entities on functors that do return void
 */
class FunctionVisitorEntitiesReturnVoid : public IVisitorEntities
{
    friend struct FunctionVisitorEntitiesBuilder<void>;
public:
    /**
     * Implementing a visit for a mob
     * @param mob mob
     */
    void visitMob(mob::Mob* mob) override { m_called = static_cast<bool>(m_visitMob);
        if (m_called) m_visitMob(mob); }
    /**
     * Implementing a visit for a player
     * @param player player
     */
    void visitPlayer(Player* player) override { m_called = static_cast<bool>(m_visitPlayer);
        if (m_called) m_visitPlayer(player); }
    /**
     * Implementing a visit for a item
     * @param item item
     */
    void visitItem(BaseItem* item) override { m_called = static_cast<bool>(m_visitItem);
        if (m_called) m_visitItem(item); }
    /**
     * Implementing a visit for a chest
     * @param chest chest
     */
    void visitChest(Chest* chest) override { m_called = static_cast<bool>(m_visitChest);
        if (m_called) m_visitChest(chest); }
    /**
     * Implementing a visit for a decoration
     * @param decoration decoration
     */
    void visitDecoration(Decoration* decoration) override { m_called = static_cast<bool>(m_visitDecoration);
        if (m_called) m_visitDecoration(decoration); }

    /**
     * Returns the status of whether the function was called during the last visit
     * @return true if the function was called
     * @return false if the function was not called
     */
    bool isCalled() const { return m_called; }
private:
    FunctionVisitorEntitiesReturnVoid(
            std::function<void(mob::Mob*)>&& visitMob,
            std::function<void(Player*)>&& visitPlayer,
            std::function<void(BaseItem*)>&& visitItem,
            std::function<void(Chest*)>&& visitChest,
            std::function<void(Decoration*)>&& visitDecoration)
        : m_visitMob(std::move(visitMob))
        , m_visitPlayer(std::move(visitPlayer))
        , m_visitItem(std::move(visitItem))
        , m_visitChest(std::move(visitChest))
        , m_visitDecoration(std::move(visitDecoration)) { }
    
    std::function<void(mob::Mob*)> m_visitMob;
    std::function<void(Player*)> m_visitPlayer;
    std::function<void(BaseItem*)> m_visitItem;
    std::function<void(Chest*)> m_visitChest;
    std::function<void(Decoration*)> m_visitDecoration;

    bool m_called = false;
};

template<typename ReturnType>
concept is_void = std::same_as<ReturnType, void>;

/**
 * Builder to create a visitor for an entity based on functions
 * @tparam ReturnType return type of functors
 */
template<typename ReturnType>
struct FunctionVisitorEntitiesBuilder {
    /**
     * Sets the functor to visit the mob
     * @param func functor
     * @return yourself
     */
    FunctionVisitorEntitiesBuilder& setMob(const std::function<ReturnType(mob::Mob* mob)>& func)
        { m_visitMob = func; return *this; }
    /**
     * Sets the functor to visit the player
     * @param func functor
     * @return yourself
     */
    FunctionVisitorEntitiesBuilder& setPlayer(const std::function<ReturnType(Player* player)>& func)
        { m_visitPlayer = func; return *this;  }
    /**
     * Sets the functor to visit the item
     * @param func functor
     * @return yourself
     */
    FunctionVisitorEntitiesBuilder& setItem(const std::function<ReturnType(BaseItem* item)>& func)
        { m_visitItem = func; return *this;  }
    /**
     * Sets the functor to visit the chest
     * @param func functor
     * @return yourself
     */
    FunctionVisitorEntitiesBuilder& setChest(const std::function<ReturnType(Chest* chest)>& func)
        { m_visitChest = func; return *this;  }
    /**
     * Sets the functor to visit the decoration
     * @param func functor
     * @return yourself
     */
    FunctionVisitorEntitiesBuilder& setDecoration(const std::function<ReturnType(Decoration* decoration)>& func)
        { m_visitDecoration = func; return *this;  }

    /**
     * Sets the functor to visit the mob
     * @param func functor
     * @return yourself
     */
    FunctionVisitorEntitiesBuilder& setMob(std::function<ReturnType(mob::Mob* mob)>&& func)
        { m_visitMob = std::move(func); return *this; }
    /**
     * Sets the functor to visit the player
     * @param func functor
     * @return yourself
     */
    FunctionVisitorEntitiesBuilder& setPlayer(std::function<ReturnType(Player* mob)>&& func)
        { m_visitPlayer = std::move(func); return *this; }
    /**
     * Sets the functor to visit the item
     * @param func functor
     * @return yourself
     */
    FunctionVisitorEntitiesBuilder& setItem(std::function<ReturnType(BaseItem* mob)>&& func)
        { m_visitItem = std::move(func); return *this; }
    /**
     * Sets the functor to visit the chest
     * @param func functor
     * @return yourself
     */
    FunctionVisitorEntitiesBuilder& setChest(std::function<ReturnType(Chest* mob)>&& func)
        { m_visitChest = std::move(func); return *this; }
    /**
     * Sets the functor to visit the decoration
     * @param func functor
     * @return yourself
     */
    FunctionVisitorEntitiesBuilder& setDecoration(std::function<ReturnType(Decoration* mob)>&& func)
        { m_visitDecoration = std::move(func); return *this; }
    
    /**
     * Builds from previously passed functors -- a functor-based visitor
     * @return functor-based visitor (with return type which not void)
     */
    std::shared_ptr<FunctionVisitorEntitiesReturnVoid> build()
        requires (std::is_void_v<ReturnType>) { return std::shared_ptr<FunctionVisitorEntitiesReturnVoid>(
        new FunctionVisitorEntitiesReturnVoid(std::move(m_visitMob), std::move(m_visitPlayer), std::move(m_visitItem),
            std::move(m_visitChest), std::move(m_visitDecoration))); }

    /**
     * Builds from previously passed functors -- a functor-based visitor
     * @return functor-based visitor (with void return type)
     */
    std::shared_ptr<FunctionVisitorEntities<ReturnType>> build()
        requires (!std::is_void_v<ReturnType>) { return std::shared_ptr<FunctionVisitorEntities<ReturnType>>(
        new FunctionVisitorEntities<ReturnType>(std::move(m_visitMob), std::move(m_visitPlayer), std::move(m_visitItem),
            std::move(m_visitChest), std::move(m_visitDecoration))); }

private:
    std::function<ReturnType(mob::Mob*)> m_visitMob;
    std::function<ReturnType(Player*)> m_visitPlayer;
    std::function<ReturnType(BaseItem*)> m_visitItem;
    std::function<ReturnType(Chest*)> m_visitChest;
    std::function<ReturnType(Decoration*)> m_visitDecoration;
};
