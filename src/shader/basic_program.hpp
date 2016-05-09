#pragma once

#include "platform/gl.hpp"
#include "shader.hpp"

#include <vector>
#include <glm/mat4x4.hpp>

namespace indigo
{
enum uniform_location
{
	uniform_projection,
	uniform_view,
	uniform_model,
	uniform_light_1_position,
	uniform_light_1_color,

	__uniform_last__
};

class basic_shader_program
{
public:
	virtual ~basic_shader_program();

	basic_shader_program(const basic_shader_program&) = delete;
	basic_shader_program& operator=(const basic_shader_program&) = delete;

	GLuint id() const {return id_;}

	void use() const;

	void model(glm::mat4 const& value);
	void view(glm::mat4 const& value);
	void projection(glm::mat4 const& value);

	virtual void sun(glm::vec3 const& position, glm::vec3 const& color) {}

protected:
	basic_shader_program();

	void uniform(uniform_location location, int value);
	void uniform(uniform_location location, float value);
	void uniform(uniform_location location, const glm::vec2& value);
	void uniform(uniform_location location, const glm::vec3& value);
	void uniform(uniform_location location, const glm::mat4& value);
	void uniform(uniform_location location, const std::vector<glm::mat4>& values);

	bool link();
	void attach_shader(shader& s);

	GLint locate_uniform(const std::string& name) const;
	GLint locate_attribute(const std::string& name) const;

private:
    GLuint id_;
	std::vector<GLint> uniform_locations_;

	void load_uniform_locations();
};
}
