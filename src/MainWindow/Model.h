#pragma once

#include <vector>

#include <Image.h>

class Model
{
private:
   std::vector<Image> _container;
public:
    void addImage(Image&& img);

    Model();
    ~Model() = default;
};

