#pragma once

#include <vector>
#include <memory>

#include <Image.h>

class Command;

class Model
{
private:
   std::vector<Image> _container;
   std::size_t _current = 0;

   std::vector<std::unique_ptr<Command>> _undo;
   std::vector<std::unique_ptr<Command>> _redo;

public:
    void addImage(Image&& img);

    Model();
    ~Model() = default;

    const Image& currentImage();
    void execute(std::unique_ptr<Command> cmd);
    bool redo();
    bool undo();

};

