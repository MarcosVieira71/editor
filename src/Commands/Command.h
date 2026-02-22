#pragma once

class ImageData;

class Command {
public:
    virtual ~Command() = default;
    virtual void apply(ImageData&) = 0;
    virtual void undo(ImageData&) = 0;

    void setTargetId(size_t id) { _targetId = id; }
    size_t targetId() const { return _targetId; }

private:
    size_t _targetId{};
};