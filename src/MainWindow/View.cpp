#include "View.h"

#include <QFile>
#include <QUiLoader>
#include <QFileDialog>
#include <QGraphicsView>
#include <QObject>
#include <QMenu>
#include <QAction>
#include <QMessageBox>

#include <core/Image.h>

#include <yarui/reactive/ObservableMap.h>
#include <yarui/ui/ContextMenu.h>
#include <yarui/ui/UiWidget.h>

#include "ToQImageAdapter.h"

#include "Model.h"

#include <stdexcept>
View::View() 
    : _window(UiWidget("ui/MainWindow.ui"))
    , _treeWidget(TreeWidget(_window->findChild<QTreeWidget*>("treeWidget")))
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

    QObject::connect( _openAction,
    &QAction::triggered,
    [this]()
    {
        _openCb();
    }
);
}

void View::show()
{
    _window->show();
}

QGraphicsScene* View::scene()
{
    return &_scene;
}

void View::setOpenCallback(std::function<void()>&& openCb)
{
    _openCb = std::move(openCb);
}

void View::setImage(const Image& img)
{
    auto pixmap = QPixmap::fromImage(toQImage(img));
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

void View::bindModel(Model& model)
{
    _treeWidget.bindContainer<Image>(model.images(), [](const Image& img){return img.path();});
    _treeWidget.bindSelection(
        model.selection(),
        [&model](std::optional<size_t> idx) {
            if (!idx)
                model.clearSelection();
            else
                model.select(*idx);
        }
    );

}

std::optional<PathExtension> View::fileDialog(FileDialogMode mode, const std::string& title, const std::string& filters)
{
    QString selectedFilter;
    QString qPath;
    const auto qtitle = QString::fromUtf8(title.c_str());
    const auto qfilter = QString::fromUtf8(filters.c_str());

    if (mode == FileDialogMode::Open) {
        qPath = QFileDialog::getOpenFileName(
            nullptr,
            qtitle,
            "",
            qfilter,
            &selectedFilter
        );
    }
    else { 
        qPath = QFileDialog::getSaveFileName(
            nullptr,
            qtitle,
            "",
            qfilter,
            &selectedFilter
        );
    }

    if (qPath.isEmpty())
        return std::nullopt;

    QFileInfo info(qPath);
    QString extension = info.suffix().toLower();

    if (mode == FileDialogMode::Save && extension.isEmpty()) {

        if (selectedFilter.contains("png", Qt::CaseInsensitive))
            extension = "png";
        else if (selectedFilter.contains("jpeg", Qt::CaseInsensitive))
            extension = "jpg";
        else if (selectedFilter.contains("bmp", Qt::CaseInsensitive))
            extension = "bmp";

        if (!extension.isEmpty())
            qPath += "." + extension;
    }
    
    return PathExtension{
        qPath.toStdString(),
        extension.toStdString()
    };
}

void View::showInfoDialog(const std::string& title, const std::string& message)
{
    QMessageBox::information(
        nullptr,
        QString::fromUtf8(title.c_str()),
        QString::fromUtf8(message.c_str()),
        QMessageBox::Ok
    );
}
