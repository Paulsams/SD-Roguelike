#pragma once
#include<optional>

#include "ItemsSystem/Accessory.h"
#include "ItemsSystem/Weapon.h"

class Backpack : public IDamageModificator
{
    // Я не придумал, как это решать. Если я сделаю InverntoryView шаблонным, то потом начинаются проблемы
    // с тем, чтобы передать InverntoryView куда-либо имея в шаблоне не конретный Item,
    // а просто <BaseItem*> (так как я буду пытаться статический полиморфизм с динамическим перемешивать,
    // ведь в статическлом полиморфизме не может быть ковариантности/контрвариантности).
    // Поэтому просто сужу интерфейс ObservableVector и прокину везде новые ивенты.
    // Ну и также придётся на доверии верить инвертарю, что он только нужный тип будет сувать.
    template <typename T>
    struct HandlerItems
    {
        HandlerItems(size_t size)
            : observableCollection(size)
            , m_changedDelegate(CC_CALLBACK_3(HandlerItems::onChange, this))
            , m_swappedDelegate(CC_CALLBACK_2(HandlerItems::onSwapped, this))
            , m_items(std::vector<T>(size))
        {
            observableCollection.changed += m_changedDelegate;
            observableCollection.swapped += m_swappedDelegate;
        }

        const std::vector<T>& get() const { return m_items; }

        EventContainer<size_t, typename ObservableVector<T>::oldValue, typename ObservableVector<T>::newValue> changed;
        EventContainer<size_t, size_t> swapped;
        ObservableVector<BaseItem*> observableCollection;

    private:
        void onChange(size_t index, ObservableVector<BaseItem*>::oldValue,
            ObservableVector<BaseItem*>::newValue newValue)
        {
            T oldValue = m_items[index];
            m_items[index] = dynamic_cast<T>(newValue);
            changed(index, oldValue, m_items[index]);
        }
        
        void onSwapped(size_t oldIndex, size_t newIndex)
            { std::swap(m_items[oldIndex], m_items[newIndex]); swapped(oldIndex, newIndex); }
    
        FunctionHandler<size_t, ObservableVector<BaseItem*>::oldValue, ObservableVector<BaseItem*>::newValue> m_changedDelegate;
        FunctionHandler<size_t, size_t> m_swappedDelegate;

        std::vector<T> m_items;
    };
    
public:
    Backpack()
        : m_changedDelegate(CC_CALLBACK_3(Backpack::onChangeWeapon, this))
        , m_weapon(1)
        , m_accessories(3)
        , m_spells(4)
    {
        m_weapon.changed += m_changedDelegate;
    }

    ObservableVector<BaseItem*>& getObservableWeapons() { return m_weapon.observableCollection; }
    ObservableVector<BaseItem*>& getObservableAccessories() { return m_accessories.observableCollection; }
    ObservableVector<BaseItem*>& getObservableSpells() { return m_spells.observableCollection; }

    Weapon* getCurrentWeapon() const
    {
        return m_weapon.get()[0];
    }
    
    float modify(float damage, BaseEntity* entity) const override
    {
        return damage;
    }

private:
    void onChangeWeapon(size_t index, ObservableVector<Weapon*>::oldValue oldValue,
        ObservableVector<Weapon*>::newValue newValue)
    {
        if (oldValue)
            oldValue->setModificatorDamage(nullptr);
        if (newValue)
            newValue->setModificatorDamage(this);
    }

    FunctionHandler<size_t, ObservableVector<Weapon*>::oldValue, ObservableVector<Weapon*>::newValue> m_changedDelegate;
    
    HandlerItems<Weapon*> m_weapon;
    HandlerItems<Accessory*> m_accessories;
    HandlerItems<Weapon*> m_spells;
};
