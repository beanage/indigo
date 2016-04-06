#pragma once

#include "platform/gl.hpp"

#include <string>
#include <memory>

namespace indigo {
class shader
{
public:
    shader(const std::string& source, GLuint type);
    ~shader();

    GLuint handle() const;

private:
    struct impl;
    std::shared_ptr<impl> shared;
};

shader load_shader(const std::string& filename, GLuint type);
}
