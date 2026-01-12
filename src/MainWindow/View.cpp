#include "View.h"

#include <QFile>
#include <QUiLoader>
#include <QGraphicsView>

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
}

void View::show()
{
    _window->show();
}

QGraphicsScene* View::scene()
{
    return _scene.get();
}

View::~View()
{
}
