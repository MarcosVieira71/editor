#pragma once

class ImageData;

class Command {
public:
    virtual ~Command() = default;
    virtual void apply(ImageData&) = 0;
    virtual void undo(ImageData&) = 0;

    void setTargetIndex(size_t idx) { _targetIndex = idx; }
    size_t targetIndex() const { return _targetIndex; }

private:
    size_t _targetIndex{};
};