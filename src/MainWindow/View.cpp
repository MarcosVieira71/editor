#include "View.h"

#include <QFile>
#include <QUiLoader>
#include <QGraphicsView>
#include <QObject>
#include <QMenu>
#include <QAction>

#include <core/Image.h>
#include <yarui/ui/ContextMenu.h>

#include <stdexcept>

View::View() : _scene(QGraphicsScene())
{
    QUiLoader loader;
    QFile file("ui/MainWindow.ui");
    if (!file.open(QFile::ReadOnly))
        throw std::runtime_error("Error loading MainWindow.ui");
 
    auto w = loader.load(&file);

    file.close();

    if (!w)
        throw std::runtime_error("Error creating MainWindow widget");

    _window = w;
    auto graphicsView = _window->findChild<QGraphicsView*>("graphicsView");
    if (!graphicsView)
        throw std::runtime_error("graphicsView not found in UI");
    
    _treeWidget = TreeWidget<Image>(_window->findChild<QTreeWidget*>("treeWidget"));

    graphicsView->setScene(&_scene);

    graphicsView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    graphicsView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    graphicsView->setTransformationAnchor(QGraphicsView::AnchorViewCenter);
    graphicsView->setResizeAnchor(QGraphicsView::AnchorViewCenter);
    graphicsView->setRenderHint(QPainter::SmoothPixmapTransform);
    
    auto openAction = _window->findChild<QAction*>("actionOpen");
    if (!openAction)
        throw std::runtime_error("actionOpen not found");
    _openAction = openAction; 

    _sceneContextMenu.setWidget(graphicsView);
    setupSceneContextMenu();
    

    connectActions();
}

void View::show()
{
    _window->show();
}

QGraphicsScene* View::scene()
{
    return &_scene;
}

void View::setActionCallback(std::function<void(ViewAction)> cb)
{
    _actionCb = std::move(cb);
}

void View::connectActions()
{
    QObject::connect(
        _openAction,
        &QAction::triggered,
        [this]() {
            if (_actionCb)
                _actionCb(ViewAction::Open);
        }
    );
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


void View::setupSceneContextMenu()
{
    _sceneContextMenu.addAction("Inversion", [this](){_actionCb(ViewAction::Inversion);});
    _sceneContextMenu.addAction("Greyscale", [this](){_actionCb(ViewAction::Grayscale);});
    _sceneContextMenu.addAction("Fit Image", [this](){_actionCb(ViewAction::FitImage);});
    _sceneContextMenu.addAction("Undo", [this](){_actionCb(ViewAction::Undo);});
    _sceneContextMenu.addAction("Redo", [this](){_actionCb(ViewAction::Redo);});
}

void View::bindModel(ObservableContainer<Image>& images)
{
    _treeWidget.bindContainer(images, [](const Image& img){return img.path();});
}