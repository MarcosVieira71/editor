#pragma once
#include <QWidget>

#include <yarui/reactive/ObservableValue.h>

#include <yarui/reactive/Subscription.h>
#include <memory>

#include <list>

namespace yarui
{

class ReactiveWidget {

public:
    ReactiveWidget(QWidget* widget) : _widget(widget) {}
    virtual ~ReactiveWidget() = default;

    template<typename Obs>
    void hide(Obs&& hideObs) {
        subs.emplace_back(std::forward<Obs>(hideObs).subscribe([this](auto& v){
            _isHidden = v;
            if (_widget) _widget->setHidden(_isHidden);
        }));
    }

    template<typename Obs>
    void enable(Obs&& enableObs)
    {
        subs.emplace_back(std::forward<Obs>(enableObs).subscribe([this](auto& v){
            _isEnabled = v;
            if (_widget) _widget->setEnabled(_isEnabled);
        }));
    }

    QWidget* widget()
    {
        return _widget;
    }

protected:

    bool _isHidden = false, _isEnabled = true;


private:
    QWidget* _widget;
    std::list<Subscription> subs;
    
};

}
