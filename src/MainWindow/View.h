#pragma once

#include <QWidget>
#include <QGraphicsScene>
#include <QAction>

#include <functional>

class View
{
private:
    std::unique_ptr<QWidget> _window;
    std::unique_ptr<QGraphicsScene> _scene;
    std::unique_ptr<QAction> _openAction;

public:

    QGraphicsScene* scene();
    void setOpenMenuCallback(std::function<void()> f);

    void show();
    void fitImage();
    View();
    ~View() = default;
};