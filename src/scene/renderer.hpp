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
    class __run;

    class renderer
    {
        friend class __run;

    public:
        void render(camera const& cam, scene& scene_);
        void fetch(basic_shader_program& target);

        float const time;

    private:
        renderer(float time);
        ~renderer();

        void render(entity& ent, glm::mat4 const& parent);

        struct impl;
        std::unique_ptr<impl> impl_;
    };
}
