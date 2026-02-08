#include "Model.h"

#include "Commands/Command.h"

#include <stdexcept>

Model::Model()
{
}

void Model::addImage(Image&& img)
{
    _container.add(std::move(img));
    _current = _container.size() - 1; //last index added
}

bool Model::execute(std::unique_ptr<Command> cmd)
{
    if(!isImageSelected()) return false;

    cmd->apply(_container[*_current]);
    _undo.push_back(std::move(cmd));
    _redo.clear();
    return true;
}

bool Model::redo()
{
    if (_redo.empty())
        return false;

    auto cmd = std::move(_redo.back());
    _redo.pop_back();

    //if this happens, something went very wrong
    if(!_current.has_value()) throw std::runtime_error("Invalid state");
    cmd->apply(_container[*_current]);
    _undo.push_back(std::move(cmd));

    return true;
}


bool Model::undo()
{
    if (_undo.empty())
        return false;

    auto cmd = std::move(_undo.back());
    _undo.pop_back();

    //if this happens, something went very wrong
    if(!_current.has_value()) throw std::runtime_error("Invalid state");
    cmd->undo(_container[*_current]);
    _redo.push_back(std::move(cmd));

    return true;
}

std::optional<const Image*> Model::currentImage() const
{
    if (!_current)
        return std::nullopt;

    return &_container[*_current];
}


bool Model::isImageSelected()
{
    return _current.has_value();
}


ObservableContainer<Image>& Model::images()
{
    return _container;
}

const ObservableContainer<Image>& Model::images() const
{
    return _container;
}
