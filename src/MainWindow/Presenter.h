#pragma once

#include <memory>

class View;
class Model;

class Presenter
{
private:
    std::unique_ptr<View> _view;
    std::unique_ptr<Model> _model;
    void onOpenImage();

public:
    Presenter();
    ~Presenter();
    void start();
    void onInversion();
    void onGrayscale();
    void onUndo();
    void onRedo();

};

