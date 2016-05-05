#pragma once

#include "shader/basic_program.hpp"

namespace  indigo {
    class default_program : public basic_shader_program  {
    public:
        default_program();

        void light1(glm::vec3 const& position, glm::vec3 const& color);
    };
}
