#include "Model.h"

#include "Commands/Command.h"

#include <stdexcept>

Model::Model()
{
}

void Model::addImage(Image&& img)
{
    _container.add(std::move(img));
    _current.set(_container.size() - 1); //last index added
}

bool Model::execute(std::unique_ptr<Command> cmd)
{
    if (!isImageSelected())
        return false;

    size_t idx = *_current.get();

    cmd->setTargetIndex(idx);
    cmd->apply(_container[idx]);

    _undo.push_back(std::move(cmd));
    _redo.clear();
    return true;
}


std::optional<size_t> Model::redo()
{
    if (_redo.empty())
        return std::nullopt;

    auto cmd = std::move(_redo.back());
    _redo.pop_back();

    std::size_t idx = cmd->targetIndex();

    cmd->apply(_container[idx]);
    _undo.push_back(std::move(cmd));

    return idx;
}

std::optional<size_t> Model::undo()
{
    if (_undo.empty())
        return std::nullopt;

    auto cmd = std::move(_undo.back());
    _undo.pop_back();

    std::size_t idx = cmd->targetIndex();

    cmd->undo(_container[idx]);
    _redo.push_back(std::move(cmd));

    return idx;
}


std::optional<std::reference_wrapper<const Image>> Model::currentImage() const
{
    const auto& idx = _current.get();

    if (!idx)
        return std::nullopt;

    return std::cref(_container[*idx]);
}



bool Model::isImageSelected()
{
    return _current.get().has_value();
}


ObservableValue<std::optional<size_t>>& Model::selection()
{
    return _current;
};

ObservableContainer<Image>& Model::images()
{
    return _container;
}

const ObservableContainer<Image>& Model::images() const
{
    return _container;
}

void Model::select(size_t index) 
{
    if (index >= _container.size())
        throw std::out_of_range("Invalid index");

    if(_current.get() == index)
        return;
        
    _current.set(index);
}


void Model::clearSelection()
{
    if (!_current.get().has_value())
        return;

    _current.set(std::nullopt);
}
