#include "default_program.hpp"
#include "platform/resource_manager.hpp"
#include "shader/shader.hpp"

indigo::default_program::default_program()
{
    attach_shader(*resource_manager<shader>::shared().load("default.vert"));
    attach_shader(*resource_manager<shader>::shared().load("default.frag"));
    link();
}
