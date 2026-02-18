#pragma once

#include <memory>
#include <functional>
#include <string>

#include <yarui/reactive/ObservableMap.h>
#include <yarui/reactive/Subscription.h>

class View;
class Model;
enum class ViewAction;

class Presenter
{
private:
    std::unique_ptr<View> _view;
    std::unique_ptr<Model> _model;

    ObservableMap<std::string, std::function<void()>> _actionMap;
    
    void onOpenImage();
    void onSaveImage();
    void refresh();
    void initActionMap();
    void onInversion();
    void onGrayscale();
    void onBinarization();
    void onUndo();
    void onRedo();

    Subscription _selectionSubscription;

public:
    Presenter();
    ~Presenter();

    void start();

};

