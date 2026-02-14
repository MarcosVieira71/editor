#pragma once

#include <vector>
#include <utility>
#include <functional>
#include <algorithm>

#include <yarui/reactive/ObservableBase.h>
#include <yarui/reactive/Subscription.h>

template<typename Event>
class Observable : public ObservableBase
{
    using Observer = std::function<void(const Event&)>;

    struct Entry
    {
        std::size_t id;
        Observer observer;
    };

public:

    Observable() = default;

    ~Observable()
    {
        for (auto* sub : _subscriptions)
            sub->invalidate();
    }

    Subscription subscribe(Observer obs)
    {
        const std::size_t id = _nextId++;
        _observers.push_back({ id, std::move(obs) });

        Subscription sub(this, id);
        _subscriptions.push_back(&sub);
        return sub;
    }

protected:

    void notify(const Event& ev)
    {
        for (auto& e : _observers)
            e.observer(ev);
    }

private:

    void unsubscribe(std::size_t id) override
    {
        auto it = std::remove_if(
            _observers.begin(),
            _observers.end(),
            [id](const Entry& e) { return e.id == id; }
        );

        _observers.erase(it, _observers.end());
    }

    void detach(Subscription* sub) override
    {
        auto it = std::remove(
            _subscriptions.begin(),
            _subscriptions.end(),
            sub
        );

        _subscriptions.erase(it, _subscriptions.end());
    }

    void replaceSubscription(Subscription* oldPtr,
                             Subscription* newPtr) override
    {
        for (auto& ptr : _subscriptions)
        {
            if (ptr == oldPtr)
            {
                ptr = newPtr;
                return;
            }
        }
    }

private:

    std::size_t _nextId = 0;
    std::vector<Entry> _observers;
    std::vector<Subscription*> _subscriptions;
};
