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

class View
{
private:
    QWidget* _window;
    QGraphicsScene _scene;
    QAction* _openAction;
    std::function<void()> _openCb;
    
    TreeWidget _treeWidget;
    ContextMenu _sceneContextMenu;
    
public:
    QGraphicsScene* scene();
    void bindActionMap(ObservableMap<std::string, std::function<void()>>& map);
    void fitImage();
    void show();
    void setImage(const QPixmap& pixmap);
    void bindModel(ObservableContainer<Image>& images);
    void setOpenCallback(std::function<void()>&& openCb);

    View();
    ~View() = default;
};