#include <yarui/reactive/Observable.h>

template<typename T>
class ObservableValue : public Observable<T>
{
public:
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

    ObservableValue& operator=(T value)
    {
        set(std::move(value));
        return *this;
    }

private:
    T _value{};
};
