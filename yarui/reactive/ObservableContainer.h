#pragma once

#include <vector>
#include <optional>
#include <functional>

#include <yarui/reactive/Observable.h>

namespace yarui
{

template<typename T>
struct ContainerEvent
{
    enum class Type {
        Added,
        Removed,
        Updated,
        Cleared
    };

    Type type;
    std::size_t index;
    std::optional<std::reference_wrapper<const T>> item;
};

template<typename T>
class ObservableContainer : public Observable<ContainerEvent<T>>
{
public:
    ObservableContainer() = default;

    ObservableContainer(const ObservableContainer&) = delete;
    ObservableContainer& operator=(const ObservableContainer&) = delete;

    ObservableContainer(ObservableContainer&&) = delete;
    ObservableContainer& operator=(ObservableContainer&&) = delete;

    ~ObservableContainer() = default;

    template<typename TT>
    void add(TT&& item)
    {
        _container.push_back(std::forward<TT>(item));

        ContainerEvent<T> ev{ ContainerEvent<T>::Type::Added,
                              _container.size() - 1,
                              std::cref(_container.back()) };

        this->notify(ev); 
    }

    void remove(std::size_t index)
    {
        if (index >= _container.size()) return;

        _container.erase(_container.begin() + index);

        ContainerEvent<T> ev{ ContainerEvent<T>::Type::Removed,
                              index,
                              std::nullopt };

        this->notify(ev);
    }

    std::size_t size() const { return _container.size(); }
    T& operator[](std::size_t i) { return _container[i]; }
    const T& operator[](std::size_t i) const { return _container[i]; }
    const std::vector<T>& items() const { return _container; }

private:
    std::vector<T> _container;
};

}
