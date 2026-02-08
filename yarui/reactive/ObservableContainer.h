#pragma once

#include <functional>
#include <optional>
#include <vector>

#include <yarui/reactive/Subscription.h>

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
class ObservableContainer
{
    using Observer = std::function<void(const ContainerEvent<T>&)>;

    public:

        ObservableContainer() = default;

        ObservableContainer(const ObservableContainer&) = delete;
        ObservableContainer& operator=(const ObservableContainer&) = delete;

        ObservableContainer(ObservableContainer&&) = delete;
        ObservableContainer& operator=(ObservableContainer&&) = delete;

        ~ObservableContainer()
        {
            _observers.clear();
        }

        void add(const T& item)
        {
            _container.push_back(item);
            notify({
                ContainerEvent<T>::Type::Added,
                _container.size() - 1,
                std::cref(_container.back())
            });
        }

        void remove(std::size_t index)
        {
            if (index >= _container.size())
                return;

            _container.erase(_container.begin() + index);

            notify({
                ContainerEvent<T>::Type::Removed,
                index,
                std::nullopt
            });
        }

        Subscription subscribe(Observer obs)
        {
            const std::size_t id = _nextId++;
            _observers.emplace_back(id, std::move(obs));

            auto unsubscribeFunc = [observers = &_observers, id]() {
                if (!observers) return;
                auto it = std::remove_if(
                    observers->begin(),
                    observers->end(),
                    [id](auto& pair) { return pair.first == id; }
                );
                observers->erase(it, observers->end());
            };

            return Subscription{std::move(unsubscribeFunc)};
        }

        std::size_t size()
        {
            return _container.size();
        }

        T& operator[](std::size_t i) {
            return _container[i];
        }

        const T& operator[](std::size_t i) const {
            return _container[i];
        }

        const std::vector<T>& items() const
        {
            return _container;
        }

    private:

        void notify(const ContainerEvent<T>& ev)
        {
            for (auto& [id, obs] : _observers)
                obs(ev);
        }

        std::size_t _nextId = 0;
        std::vector<std::pair<std::size_t, Observer>> _observers;
        std::vector<T> _container;       
};
