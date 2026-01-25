#include "View.h"

#include <QFile>
#include <QUiLoader>
#include <QGraphicsView>
#include <QObject>
#include <QMenu>
#include <QAction>

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
    graphicsView->setContextMenuPolicy(Qt::CustomContextMenu);

    QObject::connect(
        graphicsView,
        &QWidget::customContextMenuRequested,
        [this, graphicsView](const QPoint& pos) {
            showContextMenu(graphicsView->mapToGlobal(pos));
        }
    );

    connectActions();
}

void View::show()
{
    _window->show();
}

QGraphicsScene* View::scene()
{
    return _scene.get();
}

void View::setActionCallback(std::function<void(ViewAction)> cb)
{
    _actionCb = std::move(cb);
}

void View::connectActions()
{
    QObject::connect(
        _openAction.get(),
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
        _scene->sceneRect(),
        Qt::KeepAspectRatio
    );
}


void View::showContextMenu(const QPoint& globalPos)
{
    QMenu menu;

    QAction* inversion  = menu.addAction("Inversion");
    QAction* greyscale = menu.addAction("Greyscale");
    QAction* fit = menu.addAction("Fit Image");
    QAction* undo = menu.addAction("Undo");
    QAction* redo = menu.addAction("Redo");

    QAction* selected = menu.exec(globalPos);

    if (!selected)
        return;

    if (selected == inversion)
        _actionCb(ViewAction::Inversion);
    else if (selected == greyscale)
        _actionCb(ViewAction::Grayscale);
    else if (selected == fit)
        _actionCb(ViewAction::FitImage);
    else if (selected == undo)
        _actionCb(ViewAction::Undo);
    else if (selected == redo)
        _actionCb(ViewAction::Redo);
}
