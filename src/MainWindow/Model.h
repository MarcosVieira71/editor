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
private:
   ObservableContainer<Image> _container;
   ObservableValue<std::optional<size_t>> _current;

   std::vector<std::unique_ptr<Command>> _undo;
   std::vector<std::unique_ptr<Command>> _redo;

   std::map<const std::string,int> _filenameCount;

public:

    ObservableValue<std::optional<size_t>>& selection();
    ObservableContainer<Image>& images();

    const ObservableContainer<Image>& images() const;

    void addImage(Image&& img);
    std::string generateUniqueName(const std::string& base);

    Model();
    ~Model() = default;

    std::optional<std::reference_wrapper<const ImageData>>currentImage() const;
    bool isImageSelected();
    void select(size_t index);
    void clearSelection();
    bool execute(std::unique_ptr<Command> cmd);
    std::optional<size_t> redo();
    std::optional<size_t> undo();

};

