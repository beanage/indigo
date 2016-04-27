#pragma once

#include "platform/gl.hpp"
#include "shader.hpp"

#include <vector>
#include <glm/mat4x4.hpp>

namespace indigo
{
class basic_shader_program
{
public:
	virtual ~basic_shader_program();

	basic_shader_program(const basic_shader_program&) = delete;
	basic_shader_program& operator=(const basic_shader_program&) = delete;

	GLuint id() const;
	void use() const;

	void uniform(GLint location, int value);
	void uniform(GLint location, float value);
	void uniform(GLint location, const glm::vec2& value);
	void uniform(GLint location, const glm::vec3& value);
	void uniform(GLint location, const glm::mat4& value);
	void uniform(GLint location, const std::vector<glm::mat4>& values);
	void uniform(std::string location, int value);
	void uniform(std::string location, float value);
	void uniform(std::string location, const glm::vec2& value);
	void uniform(std::string location, const glm::vec3& value);
	void uniform(std::string location, const glm::mat4& value);
	void uniform(std::string location, const std::vector<glm::mat4>& values);

protected:
	basic_shader_program();

	bool link();

	void attach_shader(shader& s);

	GLint uniform_location(const std::string& name) const;
	GLint attribute_location(const std::string& name) const;

private:
    GLuint id_;
};
}
