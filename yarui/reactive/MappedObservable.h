#pragma once

#include <yarui/reactive/Observable.h>

template<typename T, typename U>
class MappedObservable : public Observable<U>
{
public:
    using Func = std::function<U(const T&)>;

    MappedObservable(Observable<T>& source, Func func)
        : _source(source), _func(func) {}

    Subscription subscribe(typename Observable<U>::Observer obs) override
    {
        return _source.subscribe([obs, func = _func](const T& v){
            obs(func(v));
        });
    }

private:
    Observable<T>& _source;
    Func _func;
};