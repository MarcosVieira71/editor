#pragma once

#include <QWidget>
#include <QGraphicsScene>

class View
{
private:
    std::unique_ptr<QWidget> _window;
    std::unique_ptr<QGraphicsScene> _scene;

public:
    QGraphicsScene* scene();

    void show();
    View();
    ~View();
};