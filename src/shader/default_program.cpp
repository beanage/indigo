#include "default_program.hpp"
#include "platform/resource_manager.hpp"
#include "shader/shader.hpp"
#include "scene/scene.hpp"
#include "scene/renderer.hpp"

using namespace indigo;

default_program::default_program()
{
    attach_shader(*resource_manager<shader>::shared().load("default.vert"));
    attach_shader(*resource_manager<shader>::shared().load("default.frag"));

    link();
    locate_uniforms();
}

void default_program::fetch(const renderer& r)
{
    basic_shader_program::fetch(r);

    uniform(sun_dir_loc_, r.get_scene().sun().direction);
    uniform(sun_color_loc_, r.get_scene().sun().color);
}

void default_program::locate_uniforms()
{
    sun_dir_loc_ = locate_uniform("sun.direction");
    sun_color_loc_ = locate_uniform("sun.color");
}
