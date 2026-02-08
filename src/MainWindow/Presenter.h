#pragma once

#include <memory>
#include <functional>

class View;
class Model;
enum class ViewAction;

class Presenter
{
private:
    std::unique_ptr<View> _view;
    std::unique_ptr<Model> _model;

    std::unordered_map<ViewAction, std::function<void()>> _actionMap;
    
    void onOpenImage();
    void refresh();
    void initActionMap();

public:
    Presenter();
    ~Presenter();

    void start();
    void onInversion();
    void onGrayscale();
    void onUndo();
    void onRedo();

};

