#include "default_program.hpp"
#include "platform/resource_manager.hpp"
#include "shader/shader.hpp"

indigo::default_program::default_program() {
    attach_shader(*resource_manager<shader>::shared().load("default.vert"));
    attach_shader(*resource_manager<shader>::shared().load("default.frag"));
    link();
}

void indigo::default_program::light1(glm::vec3 const& position, glm::vec3 const& color) {
    uniform(uniform_light_1_position, position);
    uniform(uniform_light_1_color, color);
}

