#pragma once

#include <cstddef>

namespace yarui
{
    
class Subscription;

class ObservableBase
{
public:
    virtual void unsubscribe(std::size_t id) = 0;

    virtual ~ObservableBase() = default;
};

}