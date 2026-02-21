#pragma once
#include <QWidget>

#include <yarui/reactive/ObservableValue.h>

#include <yarui/reactive/Subscription.h>
#include <memory>

#include <list>


class ReactiveWidget {

public:
    ReactiveWidget(QWidget* widget) : _widget(widget) {}
    virtual ~ReactiveWidget() = default;

    void hide(ObservableValue<bool>& hideObs) {
        subs.emplace_back(hideObs.subscribe([this](auto& v){
            _isHidden = v;
            if (_widget) _widget->setHidden(_isHidden);
        }));
    }

    void enable(ObservableValue<bool>& enableObs) {
        subs.emplace_back(enableObs.subscribe([this](auto& v){
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
