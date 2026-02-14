#include "View.h"

#include <QFile>
#include <QUiLoader>
#include <QGraphicsView>
#include <QObject>
#include <QMenu>
#include <QAction>

#include <core/Image.h>

#include <yarui/reactive/ObservableMap.h>
#include <yarui/ui/ContextMenu.h>
#include <yarui/ui/UiWidget.h>

#include <stdexcept>
View::View() 
    : _window(UiWidget("ui/MainWindow.ui"))
    , _treeWidget(TreeWidget<Image>(_window->findChild<QTreeWidget*>("treeWidget")))
    , _scene(QGraphicsScene())
    , _sceneContextMenu(_window->findChild<QGraphicsView*>("graphicsView"))
    , _openAction(_window->findChild<QAction*>("actionOpen"))

{
    auto graphicsView = _window->findChild<QGraphicsView*>("graphicsView");
    if (!graphicsView)
        throw std::runtime_error("graphicsView not found in UI");

    graphicsView->setScene(&_scene);
    graphicsView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    graphicsView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    graphicsView->setTransformationAnchor(QGraphicsView::AnchorViewCenter);
    graphicsView->setResizeAnchor(QGraphicsView::AnchorViewCenter);
    graphicsView->setRenderHint(QPainter::SmoothPixmapTransform);
}

void View::show()
{
    _window->show();
}

QGraphicsScene* View::scene()
{
    return &_scene;
}


void View::setImage(const QPixmap& pixmap)
{
    scene()->clear();
    scene()->addPixmap(pixmap);
    scene()->setSceneRect(pixmap.rect());
    fitImage();
}

void View::fitImage()
{
    auto graphicsView = _window->findChild<QGraphicsView*>("graphicsView");
    if (!graphicsView)
        return;

    graphicsView->fitInView(
        scene()->sceneRect(),
        Qt::KeepAspectRatio
    );
}


void View::bindActionMap(ObservableMap<std::string, std::function<void()>>& map)
{
    _sceneContextMenu.bindMap(map);
}

void View::bindModel(ObservableContainer<Image>& images)
{
    _treeWidget.bindContainer(images, [](const Image& img){return img.path();});
}