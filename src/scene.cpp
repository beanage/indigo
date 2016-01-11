#include "scene.hpp"
#include "octnode.hpp"

namespace indigo
{
    struct scene::impl {
        impl(aabb const& dimensions) : octree(dimensions) {

        }

        ~impl() {}
        entity root_entity;
        root_octnode octree;
    };

    scene::scene(const aabb &dimensions) : _impl(std::unique_ptr<impl>(new impl(dimensions)))
    {
    }

    void scene::add(const std::shared_ptr<scene> &entity)
    {
        _impl->entities.push_back(entity);
        root_entity.add(entity);
    }

    std::shared_ptr<entity> scene::pick_entity(const ray &here)
    {

    }

}
