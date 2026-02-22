#include "Model.h"

#include "Commands/Command.h"

#include <stdexcept>
#include <filesystem>

Model::Model()
{
}

void Model::addImage(ImageData&& img_data)
{
    auto uniqueName = generateUniqueName(std::filesystem::path(img_data.path()).filename().string());
    auto image = Image(std::move(uniqueName), std::move(img_data), _nextId++);
    auto id = image.id();
    _container.add(std::move(image));
    _current.set(id); 
}

std::optional<std::size_t> Model::indexFromId(std::size_t id) const
{
    const auto& items = _container.items();

    for (std::size_t i = 0; i < items.size(); ++i)
        if (items[i].id() == id)
            return i;

    return std::nullopt;
}

void Model::removeImage(std::size_t id)
{
    auto idx = indexFromId(id);
    if (!idx)
        return;

    _container.remove(*idx);

    if (_current.get() == id)
        clearSelection();
}


std::string Model::generateUniqueName(const std::string& base)
{
    auto& count = _nameCount[base];
    std::string finalName = base;
    if (count > 0) finalName += "_(" + std::to_string(count) + ")";
    count++;
    return finalName;
}

bool Model::execute(std::unique_ptr<Command> cmd)
{
    if (!isImageSelected())
        return false;

    auto id = *_current.get();
    auto idx = indexFromId(id);

    if (!idx)
    return false;

    cmd->setTargetId(id);
    cmd->apply(_container[*idx].image_data());

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

    auto id = cmd->targetId();
    auto idx = indexFromId(id);

    if (!idx)
        return std::nullopt;

    cmd->apply(_container[*idx].image_data());

    _undo.push_back(std::move(cmd));

    return id;
}

std::optional<size_t> Model::undo()
{
    if (_undo.empty())
        return std::nullopt;

    auto cmd = std::move(_undo.back());
    _undo.pop_back();

    auto id = cmd->targetId();
    auto idx = indexFromId(id);

    if (!idx)
        return std::nullopt;

    cmd->undo(_container[*idx].image_data());

    _redo.push_back(std::move(cmd));

    return id;
}


std::optional<std::reference_wrapper<const ImageData>> Model::currentImage() const
{
    auto id = *_current.get();
    auto idx = indexFromId(id);

    if (!idx)
        return std::nullopt;

    return std::cref(_container[*idx].image_data());
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

void Model::select(std::size_t id)
{
    if (!indexFromId(id))
        throw std::out_of_range("Invalid id");

    if (_current.get() == id)
        return;

    _current.set(id);
}



void Model::clearSelection()
{
    if (!_current.get().has_value())
        return;
    _current.set(std::nullopt);
}
