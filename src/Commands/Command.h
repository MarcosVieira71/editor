#pragma once

class Image;

class Command {
public:
    virtual ~Command() = default;
    virtual void apply(Image&) = 0;
    virtual void undo(Image&) = 0;
};