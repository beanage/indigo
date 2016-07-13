#pragma once

#include "shader/basic_program.hpp"

namespace indigo
{
class renderer;

class default_program : public basic_shader_program
{
public:
    default_program();

    void fetch(const renderer& r) override;

private:
    GLuint sun_dir_loc_;
    GLuint sun_color_loc_;

    void locate_uniforms();
};
}
