#pragma once

#include <QWidget>
#include <QGraphicsScene>
#include <QAction>
#include <QPixmap>
#include <QPoint>

#include <yarui/ui/TreeWidget.h>
#include <yarui/ui/ContextMenu.h>

#include <functional>
#include <optional>
#include <string>

class ImageData;
class Model;
class QGraphicsView;

using PathExtension = std::pair<std::string, std::string>;
enum class FileDialogMode{Open, Save};

class View
{
private:
    QWidget* _window;
    QGraphicsScene _scene;
    QGraphicsView* _graphicsView;
    QAction* _openAction;
    std::function<void()> _openCb;
    
    TreeWidget _treeWidget;
    ContextMenu _sceneContextMenu;
    ContextMenu _imageTreeContextMenu;
    
public:
    QGraphicsScene* scene();
    void bindSceneActionMap(ObservableMap<std::string, std::function<void()>>& map);
    void bindTreeActionMap(ObservableMap<std::string, std::function<void()>>& map);
    void fitImage();
    void show();
    void setImage(const ImageData& pixmap);
    void clear();
    void bindModel(Model& model);
    void setOpenCallback(std::function<void()>&& openCb);
    std::optional<PathExtension> fileDialog(FileDialogMode mode, const std::string& title, const std::string& filters);
    void showInfoDialog(const std::string& title, const std::string& message);

    View();
    ~View() = default;
};