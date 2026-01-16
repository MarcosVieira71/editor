#include "Model.h"

Model::Model()
{
}

void Model::addImage(Image&& img)
{
    _container.push_back(std::move(img));
}
