#pragma once

#include <functional>

class Subscription {
public:
    Subscription() = default;

    explicit Subscription(std::function<void()> unsubscribe)
        : _unsubscribe(std::move(unsubscribe))
    {}

    Subscription(const Subscription&) = delete;
    Subscription& operator=(const Subscription&) = delete;

    Subscription(Subscription&& other) noexcept
        : _unsubscribe(std::move(other._unsubscribe))
    {
        other._unsubscribe = nullptr;
    }

    Subscription& operator=(Subscription&& other) noexcept {
        if (this != &other) {
            unsubscribe();
            _unsubscribe = std::move(other._unsubscribe);
            other._unsubscribe = nullptr;
        }
        return *this;
    }

    ~Subscription() {
        unsubscribe();
    }

private:
    void unsubscribe() {
        if (_unsubscribe) {
            _unsubscribe();
            _unsubscribe = nullptr;
        }
    }

    std::function<void()> _unsubscribe;
};
