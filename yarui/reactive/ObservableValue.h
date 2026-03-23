#pragma once

#include <yarui/reactive/Observable.h>
#include <yarui/reactive/MappedObservable.h>
#include <yarui/reactive/Subscription.h>

#include <list>

template<typename T>
class ObservableValue : public Observable<T>
{
public:
    using base = Observable<T>;
    using Observer = typename base::Observer;

    ObservableValue() = default;
    explicit ObservableValue(T value)
        : _value(std::move(value)) {}

    const T& get() const { return _value; }

    void set(T value)
    {
        if (_value == value)
            return;

        _value = std::move(value);
        this->notify(_value);
    }

    Subscription subscribe(Observer obs) override
    {
        auto sub = base::subscribe(obs);
        this->notify(_value);
        return sub;
    }

    ObservableValue& operator=(T value)
    {
        set(std::move(value));
        return *this;
    }

    template<typename U>
    MappedObservable<T, U> view_as(std::function<U(const T&)> func)
    {
        return MappedObservable<T, U>(*this, func);
    }

private:
    T _value{};
    std::list<Subscription> subs;
    
};