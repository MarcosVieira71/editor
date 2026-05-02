#include "View.h"

#include <QFile>
#include <QUiLoader>
#include <QFileDialog>
#include <QGraphicsView>
#include <QObject>
#include <QMenu>
#include <QAction>
#include <QMessageBox>

#include <core/image/Image.h>
#include <core/image/ImageData.h>

#include <yarui/reactive/ObservableMap.h>

#include <yarui/ui/ContextMenu.h>
#include <yarui/ui/UiWidget.h>

#include "ToQImageAdapter.h"

#include "Model.h"

#include <stdexcept>
View::View() 
    : _window(yarui::UiWidget("ui/MainWindow.ui"))
    , _treeWidget(yarui::TreeWidget(_window->findChild<QTreeWidget*>("treeWidget")))
    , _scene(QGraphicsScene())
    , _sceneContextMenu(_window->findChild<QGraphicsView*>("graphicsView"))
    , _imageTreeContextMenu(_treeWidget.widget())
    , _openAction(_window->findChild<QAction*>("actionOpen"))
    , _graphicsView(_window->findChild<QGraphicsView*>("graphicsView"))
{
    
    _graphicsView->setScene(&_scene);
    _graphicsView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    _graphicsView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    _graphicsView->setTransformationAnchor(QGraphicsView::AnchorViewCenter);
    _graphicsView->setResizeAnchor(QGraphicsView::AnchorViewCenter);
    _graphicsView->setRenderHint(QPainter::SmoothPixmapTransform);

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

void View::setImage(const image::ImageData& img)
{
    auto pixmap = QPixmap::fromImage(toQImage(img));
    scene()->clear();
    scene()->addPixmap(pixmap);
    scene()->setSceneRect(pixmap.rect());
    fitImage();
}

void View::clear()
{
    scene()->clear();
}

void View::fitImage()
{

    _graphicsView->fitInView(
        scene()->sceneRect(),
        Qt::KeepAspectRatio
    );
}


void View::bindSceneActionMap(yarui::ObservableMap<std::string, std::function<void()>>& map)
{
    _sceneContextMenu.bindMap(map);
}

void View::bindTreeActionMap(yarui::ObservableMap<std::string, std::function<void()>>& map)
{
    _imageTreeContextMenu.bindMap(map);
}

void View::bindModel(Model& model)
{
    _treeWidget.bindContainer<image::Image, std::size_t>(model.images(), [](const image::Image& img){return img.name();}, [](const image::Image& img){return img.id();});
    _treeWidget.bindSelection <std::size_t>(
        model.selection(),
        [&model](std::optional<size_t> id) {
            if (!id)
                model.clearSelection();
            else
                model.select(*id);
        }
    );
    _sceneContextMenu.enable(model.selection().view_as<bool>([](const auto& s){
        return s.has_value();}));
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
