#include "Model.h"

#include "Commands/Command.h"

#include <stdexcept>
#include <filesystem>

Model::Model()
{
}

void Model::addImage(image::ImageData&& img_data)
{
    auto uniqueName = generateUniqueName(std::filesystem::path(img_data.path()).filename().string());
    auto image = image::Image(std::move(uniqueName), std::move(img_data), _nextId++);
    auto id = image.id();
    _container.add(std::move(image));
    _history[id] = CommandHistory{};
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
    _history.erase(id);

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

    auto& history = _history[id];

    history.undo.push_back(std::move(cmd));
    history.redo.clear();
    return true;
}


std::optional<size_t> Model::redo()
{
    if(!isImageSelected())
        return std::nullopt;

    auto id = *_current.get();
    auto& history = _history[id];

    if (history.redo.empty())
        return std::nullopt;

    auto cmd = std::move(history.redo.back());
    history.redo.pop_back();

    auto idx = indexFromId(id);

    if (!idx)
        return std::nullopt;

    cmd->apply(_container[*idx].image_data());

    history.undo.push_back(std::move(cmd));

    return id;
}

std::optional<size_t> Model::undo()
{
    if(!isImageSelected())
        return std::nullopt;

    auto id = *_current.get();
    auto& history = _history[id];
    
    if (history.undo.empty())
        return std::nullopt;
    
    auto cmd = std::move(history.undo.back());
    history.undo.pop_back();

    auto idx = indexFromId(id);

    if (!idx)
        return std::nullopt;

    cmd->undo(_container[*idx].image_data());

    history.redo.push_back(std::move(cmd));

    return id;
}

std::optional<size_t> Model::revert()
{
    if(!isImageSelected())
        return std::nullopt;

    auto id = *_current.get();
    auto& history = _history[id];

    if(history.undo.empty())
        return std::nullopt;

    auto idx = indexFromId(id);
    if(!idx)
        return std::nullopt;

    while(!history.undo.empty())
    {
        auto cmd = std::move(history.undo.back());
        history.undo.pop_back();

        cmd->undo(_container[*idx].image_data());
        
        history.redo.push_back(std::move(cmd));
    }

    return id;
}


std::optional<std::reference_wrapper<const image::ImageData>> Model::currentImage() const
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


yarui::ObservableValue<std::optional<size_t>>& Model::selection()
{
    return _current;
};

yarui::ObservableContainer<image::Image>& Model::images()
{
    return _container;
}

const yarui::ObservableContainer<image::Image>& Model::images() const
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
