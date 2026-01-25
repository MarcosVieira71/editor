#pragma once

#include <QWidget>
#include <QGraphicsScene>
#include <QAction>
#include <QPixmap>
#include <QPoint>

#include <functional>

enum class ViewAction {
    Open,
    Grayscale,
    Inversion,
    FitImage,
    Undo,
    Redo
};


class View
{
private:
    std::unique_ptr<QWidget> _window;
    std::unique_ptr<QGraphicsScene> _scene;
    std::unique_ptr<QAction> _openAction;
    void showContextMenu(const QPoint& globalPos);
    void connectActions();

public:

    QGraphicsScene* scene();
    void setActionCallback(std::function<void(ViewAction)> cb);
    void fitImage();
    void show();
    void setImage(const QPixmap& pixmap);
    std::function<void(ViewAction)> _actionCb;

    View();
    ~View() = default;
};