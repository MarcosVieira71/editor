#pragma once

#include <QWidget>
#include <QGraphicsScene>
#include <QAction>
#include <QPixmap>

#include <functional>

class View
{
private:
    std::unique_ptr<QWidget> _window;
    std::unique_ptr<QGraphicsScene> _scene;
    std::unique_ptr<QAction> _openAction;
    void fitImage();

public:

    QGraphicsScene* scene();
    void setOpenMenuCallback(std::function<void()> f);

    void show();
    void setImage(const QPixmap& pixmap);

    View();
    ~View() = default;
};