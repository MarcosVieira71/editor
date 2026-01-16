#include "View.h"

#include <QFile>
#include <QUiLoader>
#include <QGraphicsView>
#include <QObject>

#include <stdexcept>

View::View() : _scene(std::make_unique<QGraphicsScene>())
{
    QUiLoader loader;
    QFile file("../ui/MainWindow.ui");
    if (!file.open(QFile::ReadOnly))
        throw std::runtime_error("Error loading MainWindow.ui");
 
    auto w = loader.load(&file);

    file.close();

    if (!w)
        throw std::runtime_error("Error creating MainWindow widget");

    _window.reset(w);
    auto graphicsView = _window->findChild<QGraphicsView*>("graphicsView");
    if (!graphicsView)
        throw std::runtime_error("graphicsView not found in UI");

    graphicsView->setScene(_scene.get());

    auto openAction = _window->findChild<QAction*>("actionOpen");
    if (!openAction)
        throw std::runtime_error("actionOpen not found");
    _openAction.reset(openAction); 

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
    return _scene.get();
}

void View::setOpenMenuCallback(std::function<void()> f)
{
    QObject::connect(
        _openAction.get(),
        &QAction::triggered,
        [cb = std::move(f)](bool) {
            cb();
        }
    );
}

void View::fitImage()
{
    auto graphicsView = _window->findChild<QGraphicsView*>("graphicsView");
    if (!graphicsView)
        return;

    graphicsView->fitInView(
        _scene->sceneRect(),
        Qt::KeepAspectRatio
    );
}
