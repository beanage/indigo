#include "basic_program.hpp"
#include "util/log.hpp"

#include <glm/gtc/type_ptr.hpp>

using namespace indigo;

basic_shader_program::basic_shader_program()
	: id_(0)
{
	id_ = glCreateProgram();
}

basic_shader_program::~basic_shader_program()
{
	glDeleteProgram(id_);
}

void basic_shader_program::use() const
{
	glUseProgram(id_);
}

bool basic_shader_program::link()
{
	glLinkProgram(id_);

	GLint status(0);
	glGetProgramiv(id_, GL_LINK_STATUS, &status);

	GLint log_len(0);
	glGetProgramiv(id_, GL_INFO_LOG_LENGTH, &log_len);

	if (!status) {
		std::string log(log_len, '\0');
		glGetProgramInfoLog(id_, static_cast<GLsizei>(log.size()), nullptr, &log[0]);

		log::write("Shader Program: ", log);
	}

    use();
	load_uniform_locations();
	return status != 0;
}

void basic_shader_program::attach_shader(shader& s)
{
	glAttachShader(id_, s.id());
}

GLint basic_shader_program::locate_attribute(const std::string &name) const
{
    GLint temp = glGetAttribLocation(id_, name.c_str());
    if (temp == -1)
        throw std::runtime_error("invalid attribute name '" + name + "'!");

    return temp;
}

GLint basic_shader_program::locate_uniform(const std::string& name) const
{
    GLint temp = glGetUniformLocation(id_, name.c_str());
    if (temp < 0)
        throw std::runtime_error("invalid uniform name '" + name + "'!");

    return temp;
}

void basic_shader_program::uniform(uniform_location location, int value)
{
    glUniform1i(uniform_locations_[location], value);
}

void basic_shader_program::uniform(uniform_location location, float value)
{
    glUniform1f(uniform_locations_[location], value);
}

void basic_shader_program::uniform(uniform_location location, const glm::vec2& value)
{
    glUniform2fv(uniform_locations_[location], 1, glm::value_ptr(value));
}

void basic_shader_program::uniform(uniform_location location, const glm::vec3& value)
{
    glUniform3fv(uniform_locations_[location], 1, glm::value_ptr(value));
}

void basic_shader_program::uniform(uniform_location location, const glm::mat4& value)
{
    glUniformMatrix4fv(uniform_locations_[location], 1, GL_TRUE, glm::value_ptr(value));
}

void basic_shader_program::uniform(uniform_location location, const std::vector<glm::mat4>& values)
{
    glUniformMatrix4fv(uniform_locations_[location], values.size(), GL_TRUE, reinterpret_cast<float const*>(values.data()));
}

void basic_shader_program::load_uniform_locations() {
	if(!uniform_locations_.empty())
		return;

	uniform_locations_.resize(__uniform_last__);
	uniform_locations_[uniform_projection] = locate_uniform("projection");
	uniform_locations_[uniform_view] = locate_uniform("view");
	uniform_locations_[uniform_model] = locate_uniform("model");
	uniform_locations_[uniform_light_1_position] = locate_uniform("light_1.position");
	uniform_locations_[uniform_light_1_color] = locate_uniform("light_1.color");
}

void basic_shader_program::model(glm::mat4 const& value) {
	uniform(uniform_model, value);
}

void basic_shader_program::view(glm::mat4 const& value) {
    uniform(uniform_view, value);
}

void basic_shader_program::projection(glm::mat4 const& value) {
    uniform(uniform_projection, value);
}