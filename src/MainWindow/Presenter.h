#pragma once

#include <memory>
#include <functional>
#include <string>

#include <yarui/reactive/ObservableMap.h>

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
    void refresh();
    void initActionMap();

public:
    Presenter();
    ~Presenter();

    void start();
    void onInversion();
    void onGrayscale();
    void onBinarization();
    void onUndo();
    void onRedo();

};

