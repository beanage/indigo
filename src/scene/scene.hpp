#pragma once

#include <memory>
#include <vec3.hpp>

#include "group_entity.hpp"
#include "global_light.hpp"

namespace indigo
{
    class entity;
    class renderer;

    class scene
    {
        friend class renderer;
    public:
        explicit scene(glm::vec3 const& size);

        scene& add(std::shared_ptr<entity> const& ent);
        scene& remove(std::shared_ptr<entity> const& ent);

        void sun(const global_light& light);
        global_light sun() const;

    private:
        scene() = delete;
        scene(const scene&) = delete;
        scene(scene&&) = delete;

        group_entity root_;
        global_light sun_;
    };
}
