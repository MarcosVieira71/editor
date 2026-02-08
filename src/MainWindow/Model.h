#pragma once

#include <vector>
#include <memory>
#include <yarui/reactive/ObservableContainer.h>
#include <optional>

#include <core/Image.h>

class Command;

class Model
{
private:
   ObservableContainer<Image> _container;
   std::optional<std::size_t> _current;

   std::vector<std::unique_ptr<Command>> _undo;
   std::vector<std::unique_ptr<Command>> _redo;

public:

    ObservableContainer<Image>& images();

    const ObservableContainer<Image>& images() const;

    void addImage(Image&& img);

    Model();
    ~Model() = default;

    std::optional<const Image*> currentImage() const;
    bool isImageSelected();
    bool execute(std::unique_ptr<Command> cmd);
    bool redo();
    bool undo();

};

