#pragma once

#include <QWidget>
#include <QGraphicsScene>
#include <QAction>
#include <QPixmap>
#include <QPoint>

#include <yarui/ui/TreeWidget.h>
#include <yarui/ui/ContextMenu.h>

#include <functional>

class Image;
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
    QWidget* _window;
    QGraphicsScene _scene;
    QAction* _openAction;
    
    TreeWidget<Image> _treeWidget;
    ContextMenu _sceneContextMenu;
    void setupSceneContextMenu();
    void connectActions();

public:

    QGraphicsScene* scene();
    void setActionCallback(std::function<void(ViewAction)> cb);
    void fitImage();
    void show();
    void setImage(const QPixmap& pixmap);
    void bindModel(ObservableContainer<Image>& images);
    std::function<void(ViewAction)> _actionCb;

    View();
    ~View() = default;
};