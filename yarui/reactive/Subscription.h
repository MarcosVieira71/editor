#pragma once

#include <cstddef>

#include <yarui/reactive/ObservableBase.h>

class Subscription {
public:
    Subscription() = default;

    Subscription(ObservableBase* owner, std::size_t id)
        : _owner(owner), _id(id)
    {}

    Subscription(const Subscription&) = delete;
    Subscription& operator=(const Subscription&) = delete;

    Subscription(Subscription&& other) noexcept
        : _owner(other._owner), _id(other._id)
    {
        if (_owner)
            _owner->replaceSubscription(&other, this);

        other._owner = nullptr;
    }

    Subscription& operator=(Subscription&& other) noexcept {
        if (this != &other) {
            unsubscribe();

            _owner = other._owner;
            _id = other._id;

            if (_owner)
                _owner->replaceSubscription(&other, this);

            other._owner = nullptr;
        }
        return *this;
    }

    ~Subscription() {
        unsubscribe();
    }

    void invalidate() {
        _owner = nullptr;
    }

    void unsubscribe() {
        if (_owner) {
            _owner->unsubscribe(_id);
            _owner->detach(this);
            _owner = nullptr;
        }
    }

private:
    ObservableBase* _owner = nullptr;
    std::size_t _id = 0;
};
