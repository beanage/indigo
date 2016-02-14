#include "terrain_mesh.hpp"

using namespace indigo;

terrain_object::terrain_object(const std::shared_ptr<mesh> &m)
    : mesh_(m)
{}

terrain_object::~terrain_object()
{}

glm::ivec2 terrain_object::tile() const
{
    return tile_;
}

void terrain_object::tile(glm::ivec2 t)
{
    tile_ = t;
}
