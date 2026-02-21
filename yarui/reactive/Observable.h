#pragma once

#include <vector>
#include <utility>
#include <functional>
#include <algorithm>

#include <yarui/reactive/ControlBlock.h>
#include <yarui/reactive/ObservableBase.h>
#include <yarui/reactive/Subscription.h>

template<typename Event>
class Observable : public ObservableBase
{
    
public:
    using Observer = std::function<void(const Event&)>;


    Observable()
    : _control(std::make_shared<ControlBlock>())
    {
        _control->owner = this;
    }

    ~Observable() = default;

    virtual Subscription subscribe(Observer obs)
    {
        const std::size_t id = _nextId++;
        _observers.push_back({ id, std::move(obs) });

        return Subscription(_control, id);
    }

protected:
    void notify(const Event& ev)
    {
        auto observers = _observers;
        for (auto& e : observers)
            e.observer(ev);
    }

private:
    struct Entry
    {
        std::size_t id;
        Observer observer;
    };
    
    void unsubscribe(std::size_t id) override
    {
        auto it = std::remove_if(
            _observers.begin(),
            _observers.end(),
            [id](const Entry& e) { return e.id == id; }
        );

        _observers.erase(it, _observers.end());
    }

private:
    std::size_t _nextId = 0;
    std::vector<Entry> _observers;

    std::shared_ptr<ControlBlock> _control;
};
