#pragma once

#include <memory>
#include "mat4x4.hpp"

namespace indigo
{
class scene;
class camera;
class entity;
class basic_shader_program;
class __run;

class renderer
{
    friend class __run;
public:
    void render(camera const& cam, scene& scene_);

    void prepare(basic_shader_program& p);
    const scene& get_scene() const;
    const camera& get_camera() const;
    const entity& get_entity() const;

    float const time;

private:
    renderer(float time);
    ~renderer();

    void render(entity& ent, glm::mat4 const& parent);

    struct render_state;
    render_state* state_;
};
}
