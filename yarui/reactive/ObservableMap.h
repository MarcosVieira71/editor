#pragma once

#include <map>
#include <optional>
#include <functional>

#include <yarui/reactive/Observable.h>

namespace yarui
{

template<typename K, typename V>
struct MapEvent
{
    enum class Type {
        Added,
        Removed,
        Cleared
    };

    Type type;
    K key;
    std::optional<std::reference_wrapper<const V>> item;
};

template<typename K, typename V>
class ObservableMap : public Observable<MapEvent<K, V>>
{
public:
    ObservableMap() = default;

    ObservableMap(const ObservableMap&) = delete;
    ObservableMap& operator=(const ObservableMap&) = delete;

    ObservableMap(ObservableMap&&) = delete;
    ObservableMap& operator=(ObservableMap&&) = delete;

    ~ObservableMap() = default;

    template<typename KK, typename VV>
    void add(KK&& key, VV&& value)
    {
        auto [it, inserted] =
            _container.insert_or_assign(
                std::forward<KK>(key),
                std::forward<VV>(value)
            );

        MapEvent<K, V> ev{
            MapEvent<K, V>::Type::Added,
            it->first,
            std::cref(it->second)
        };

        this->notify(ev);
    }

    void remove(const K& key)
    {
        auto it = _container.find(key);
        if (it == _container.end()) return;

        _container.erase(it);

        MapEvent<K, V> ev{ MapEvent<K, V>::Type::Removed,
                           key,
                           std::nullopt };

        this->notify(ev);
    }

    void clear()
    {
        _container.clear();

        MapEvent<K, V> ev{ MapEvent<K, V>::Type::Cleared,
                           K{},
                           std::nullopt };

        this->notify(ev);
    }

    std::size_t size() const { return _container.size(); }

    V& operator[](const K& key) { return _container[key]; }
    const V& operator[](const K& key) const { return _container.at(key); }

    const std::map<K, V>& items() const { return _container; }

private:
    std::map<K, V> _container;
};

}
