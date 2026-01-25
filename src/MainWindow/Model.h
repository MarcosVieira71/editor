#pragma once

#include <vector>
#include <memory>
#include <optional>

#include <Image.h>

class Command;

class Model
{
private:
   std::vector<Image> _container;
   std::optional<std::size_t> _current;

   std::vector<std::unique_ptr<Command>> _undo;
   std::vector<std::unique_ptr<Command>> _redo;

public:
    void addImage(Image&& img);

    Model();
    ~Model() = default;

    std::optional<const Image*> currentImage() const;
    bool isImageSelected();
    bool execute(std::unique_ptr<Command> cmd);
    bool redo();
    bool undo();

};

