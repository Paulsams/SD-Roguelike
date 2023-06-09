#pragma once
#include <functional>
#include <optional>

#include "IVisitorEntities.h"

template<typename ReturnType>
struct FunctionVisitorEntitiesBuilder;

template<typename ReturnType>
class FunctionVisitorEntities : public IVisitorEntities
{
    friend struct FunctionVisitorEntitiesBuilder<ReturnType>;
public:
    void visitMob(mob::Mob* mob) override { if (m_visitMob) m_returnType = m_visitMob(mob); }
    void visitPlayer(Player* player) override { if (m_visitPlayer) m_returnType = m_visitPlayer(player); }
    void visitItem(BaseItem* item) override { if (m_visitItem) m_returnType = m_visitItem(item); }
    void visitChest(Chest* chest) override { if (m_visitChest) m_returnType = m_visitChest(chest); }
    void visitDecoration(Decoration* decoration) override { if (m_visitDecoration) m_returnType = m_visitDecoration(decoration); }

    std::optional<ReturnType> getReturnValue()
    {
        std::optional<ReturnType> copied = m_returnType;
        m_returnType.reset();
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

    std::optional<ReturnType> m_returnType;
};

class FunctionVisitorEntitiesReturnVoid : public IVisitorEntities
{
    friend struct FunctionVisitorEntitiesBuilder<void>;
public:
    void visitMob(mob::Mob* mob) override { m_called = static_cast<bool>(m_visitMob);
        if (m_called) m_visitMob(mob); }
    void visitPlayer(Player* player) override { m_called = static_cast<bool>(m_visitPlayer);
        if (m_called) m_visitPlayer(player); }
    void visitItem(BaseItem* item) override { m_called = static_cast<bool>(m_visitItem);
        if (m_called) m_visitItem(item); }
    void visitChest(Chest* chest) override { m_called = static_cast<bool>(m_visitChest);
        if (m_called) m_visitChest(chest); }
    void visitDecoration(Decoration* decoration) override { m_called = static_cast<bool>(m_visitDecoration);
        if (m_called) m_visitDecoration(decoration); }

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

template<typename ReturnType>
struct FunctionVisitorEntitiesBuilder {
    FunctionVisitorEntitiesBuilder& setMob(const std::function<ReturnType(mob::Mob* mob)>& func)
        { m_visitMob = func; return *this; }
    FunctionVisitorEntitiesBuilder& setPlayer(const std::function<ReturnType(Player* mob)>& func)
        { m_visitPlayer = func; return *this;  }
    FunctionVisitorEntitiesBuilder& setItem(const std::function<ReturnType(BaseItem* mob)>& func)
        { m_visitItem = func; return *this;  }
    FunctionVisitorEntitiesBuilder& setChest(const std::function<ReturnType(Chest* mob)>& func)
        { m_visitChest = func; return *this;  }
    FunctionVisitorEntitiesBuilder& setDecoration(const std::function<ReturnType(Decoration* mob)>& func)
        { m_visitDecoration = func; return *this;  }

    FunctionVisitorEntitiesBuilder& setMob(std::function<ReturnType(mob::Mob* mob)>&& func)
        { m_visitMob = std::move(func); return *this; }
    FunctionVisitorEntitiesBuilder& setPlayer(std::function<ReturnType(Player* mob)>&& func)
        { m_visitPlayer = std::move(func); return *this; }
    FunctionVisitorEntitiesBuilder& setItem(std::function<ReturnType(BaseItem* mob)>&& func)
        { m_visitItem = std::move(func); return *this; }
    FunctionVisitorEntitiesBuilder& setChest(std::function<ReturnType(Chest* mob)>&& func)
        { m_visitChest = std::move(func); return *this; }
    FunctionVisitorEntitiesBuilder& setDecoration(std::function<ReturnType(Decoration* mob)>&& func)
        { m_visitDecoration = std::move(func); return *this; }
    
    std::shared_ptr<FunctionVisitorEntitiesReturnVoid> build()
        requires (std::is_void_v<ReturnType>) { return std::shared_ptr<FunctionVisitorEntitiesReturnVoid>(
        new FunctionVisitorEntitiesReturnVoid(std::move(m_visitMob), std::move(m_visitPlayer), std::move(m_visitItem),
            std::move(m_visitChest), std::move(m_visitDecoration))); }
    
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
