#pragma once

#include <memory>

class View;

class Presenter
{
private:
    std::unique_ptr<View> _view;
    void onOpenImage();

public:
    Presenter();
    ~Presenter();
    void start();

};

