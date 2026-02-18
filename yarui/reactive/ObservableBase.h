#pragma once

#include <cstddef>

class Subscription;

class ObservableBase
{
public:
    virtual void unsubscribe(std::size_t id) = 0;

    virtual ~ObservableBase() = default;
};
