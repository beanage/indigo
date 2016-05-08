#pragma once

#include <memory>
#include "mat4x4.hpp"

namespace indigo
{
    class scene;
    class camera;
    class entity;
    class application;
    class basic_shader_program;

    class renderer
    {
        friend class application;

    public:
        void render(camera const& cam, scene& scene_);
        void fetch(basic_shader_program& target);

    private:
        renderer(float time);
        void render(entity& ent, glm::mat4 const& parent);

        struct impl;
        std::unique_ptr<impl> impl_;
    };
}
