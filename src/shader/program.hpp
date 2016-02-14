#ifndef __PROGRAM_HPP_INCLUDED__
#define __PROGRAM_HPP_INCLUDED__

#include "platform/gl.hpp"
#include "shader.hpp"

#include <vector>
#include <glm/mat4x4.hpp>

namespace indigo {
class program
{
public:
    program(const std::vector<shader>& shaders);
    ~program();

    void use() const;

    GLint attribute(const std::string& name) const;
    GLint uniform(const std::string& name) const;

    void set(const std::string& name, int value);
    void set(const std::string& name, float value);
    void set(const std::string& name, const glm::vec2& value);
    void set(const std::string& name, const glm::vec3& value);
    void set(const std::string& name, const glm::mat4& value);
    void set(const std::string& name, const std::vector<glm::mat4>& values);

private:
    GLuint handle;

    program(const program&) = delete;
    program& operator=(const program&) = delete;
};
}

#endif
