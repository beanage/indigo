#include "scene.hpp"

using namespace indigo;

scene::scene(glm::vec3 const& size)
{}

scene& scene::add(const std::shared_ptr<entity>& ent)
{
    root_.add(ent);
    return *this;
}

scene& scene::remove(const std::shared_ptr<entity>& ent)
{
    root_.remove(ent);
    return *this;
}

global_light scene::sun() const
{
    return sun_;
}

void scene::sun(global_light const &s)
{
    sun_ = s;
}

