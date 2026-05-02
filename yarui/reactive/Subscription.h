#pragma once

#include <cstddef>
#include <memory>

#include <yarui/reactive/ObservableBase.h>

namespace yarui
{

class Subscription {
public:
    Subscription() = default;

    Subscription(std::shared_ptr<ControlBlock> control, std::size_t id)
        : _control(control)
        , _id(id)
    {}

    Subscription(const Subscription&) = delete;
    Subscription& operator=(const Subscription&) = delete;

    Subscription(Subscription&&) noexcept = default;
    Subscription& operator=(Subscription&&) noexcept = default;

    ~Subscription() 
    {
        unsubscribe();
    }

    void unsubscribe() {
        if (auto ctrl = _control.lock()) {
            ctrl->owner->unsubscribe(_id);
        }
        _control.reset();
    }

private:
    std::weak_ptr<ControlBlock> _control;
    std::size_t _id = 0;
};
}
