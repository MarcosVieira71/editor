#pragma once

#include <core/image/ImageData.h>
#include <core/image/Image.h>
#include <yarui/reactive/ObservableContainer.h>
#include <yarui/reactive/ObservableValue.h>

#include <vector>
#include <memory>
#include <optional>
#include <map>
#include <string>

class Command;

class Model
{
    
    public:
    
    yarui::ObservableValue<std::optional<size_t>>& selection();
    yarui::ObservableContainer<image::Image>& images();
    
    const yarui::ObservableContainer<image::Image>& images() const;
    
    void addImage(image::ImageData&& img);
    void removeImage(std::size_t idx);
    std::optional<std::size_t> indexFromId(std::size_t id) const;
    
    Model();
    ~Model() = default;
    
    std::optional<std::reference_wrapper<const image::ImageData>>currentImage() const;
    bool isImageSelected();
    void select(size_t index);
    void clearSelection();
    bool execute(std::unique_ptr<Command> cmd);
    std::optional<size_t> redo();
    std::optional<size_t> undo();
    std::optional<size_t> revert();

    private:
    
    yarui::ObservableContainer<image::Image> _container;
    yarui::ObservableValue<std::optional<size_t>> _current;

    using CommandStack = std::vector<std::unique_ptr<Command>>;
    struct CommandHistory
    {
        CommandStack undo;
        CommandStack redo;
    };

    std::map<std::size_t, CommandHistory> _history;

    std::map<const std::string,std::size_t> _nameCount;

    std::size_t _nextId;

    std::string generateUniqueName(const std::string& base);
};

