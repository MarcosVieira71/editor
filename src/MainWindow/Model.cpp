#include "Model.h"

#include "Commands/Command.h"

Model::Model()
{
}

void Model::addImage(Image&& img)
{
    _container.push_back(std::move(img));
}

void Model::execute(std::unique_ptr<Command> cmd)
{
    cmd->apply(_container[_current]);
    _undo.push_back(std::move(cmd));
    _redo.clear();
}

bool Model::redo()
{
    if (_redo.empty())
        return false;

    auto cmd = std::move(_redo.back());
    _redo.pop_back();

    cmd->apply(_container[_current]);
    _undo.push_back(std::move(cmd));

    return true;
}


bool Model::undo()
{
    if (_undo.empty())
        return false;

    auto cmd = std::move(_undo.back());
    _undo.pop_back();

    cmd->undo(_container[_current]);
    _redo.push_back(std::move(cmd));

    return true;
}

const Image& Model::currentImage()
{
    return _container[_current];
}