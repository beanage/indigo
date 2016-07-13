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

	GLuint id() const {return id_;}

	void use() const;

	void model(glm::mat4 const& value);
	void view(glm::mat4 const& value);
	void projection(glm::mat4 const& value);

protected:
	basic_shader_program();

	void uniform(GLuint location, int value);
	void uniform(GLuint location, float value);
	void uniform(GLuint location, const glm::vec2& value);
	void uniform(GLuint location, const glm::vec3& value);
	void uniform(GLuint location, const glm::mat4& value);
	void uniform(GLuint location, const std::vector<glm::mat4>& values);

	bool link();
	void attach_shader(shader& s);

	GLint locate_uniform(const std::string& name) const;
	GLint locate_attribute(const std::string& name) const;

private:
    basic_shader_program(const basic_shader_program&) = delete;
    basic_shader_program(basic_shader_program&&) = delete;

    GLuint id_;
    GLuint model_loc_;
    GLuint view_loc_;
    GLuint proj_loc_;

	void locate_uniforms();
};
}
