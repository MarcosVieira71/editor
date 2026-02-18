#pragma once

#include <core/Image.h>
#include <yarui/reactive/ObservableContainer.h>
#include <yarui/reactive/ObservableValue.h>

#include <vector>
#include <memory>
#include <optional>

class Command;

class Model
{
private:
   ObservableContainer<Image> _container;
   ObservableValue<std::optional<size_t>> _current;

   std::vector<std::unique_ptr<Command>> _undo;
   std::vector<std::unique_ptr<Command>> _redo;

public:

    ObservableValue<std::optional<size_t>>& selection();
    ObservableContainer<Image>& images();

    const ObservableContainer<Image>& images() const;

    void addImage(Image&& img);

    Model();
    ~Model() = default;

    std::optional<std::reference_wrapper<const Image>>currentImage() const;
    bool isImageSelected();
    void select(size_t index);
    void clearSelection();
    bool execute(std::unique_ptr<Command> cmd);
    bool redo();
    bool undo();

};

