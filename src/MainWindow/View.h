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

class Image;

using PathExtension = std::pair<std::string, std::string>;
enum class FileDialogMode{Open, Save};

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
    void setImage(const Image& pixmap);
    void bindModel(ObservableContainer<Image>& images);
    void setOpenCallback(std::function<void()>&& openCb);
    std::optional<PathExtension> fileDialog(FileDialogMode mode, const std::string& title, const std::string& filters);

    View();
    ~View() = default;
};