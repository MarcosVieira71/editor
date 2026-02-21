#pragma once

#include <yarui/reactive/Observable.h>
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
    ObservableValue<U>& view_as(std::function<U(const T&)> func)
    {
        auto derived = std::make_unique<ObservableValue<U>>();
        ObservableValue<U>* raw_ptr = derived.get();
        deriveds.emplace_back(std::move(derived));
        subs.emplace_back(this->subscribe([raw_ptr, func](const T& v){
            *raw_ptr = func(v);
        }));
        return *raw_ptr;
    }

private:
    T _value{};
    std::vector<std::unique_ptr<ObservableBase>> deriveds;
    std::list<Subscription> subs;
    
};