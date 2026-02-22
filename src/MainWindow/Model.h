#pragma once

#include <core/ImageData.h>
#include <core/Image.h>
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
    
    ObservableValue<std::optional<size_t>>& selection();
    ObservableContainer<Image>& images();
    
    const ObservableContainer<Image>& images() const;
    
    void addImage(ImageData&& img);
    void removeImage(std::size_t idx);
    std::optional<std::size_t> indexFromId(std::size_t id) const;
    
    Model();
    ~Model() = default;
    
    std::optional<std::reference_wrapper<const ImageData>>currentImage() const;
    bool isImageSelected();
    void select(size_t index);
    void clearSelection();
    bool execute(std::unique_ptr<Command> cmd);
    std::optional<size_t> redo();
    std::optional<size_t> undo();

    private:
    
    ObservableContainer<Image> _container;
    ObservableValue<std::optional<size_t>> _current;

    std::vector<std::unique_ptr<Command>> _undo;
    std::vector<std::unique_ptr<Command>> _redo;

    std::map<const std::string,std::size_t> _nameCount;

    std::size_t _nextId;

    std::string generateUniqueName(const std::string& base);
};

