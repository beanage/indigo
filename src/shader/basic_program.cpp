#include "basic_program.hpp"
#include "util/log.hpp"
#include "scene/renderer.hpp"
#include "scene/entity.hpp"
#include "scene/camera.hpp"

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

void basic_shader_program::fetch(const renderer& r)
{
    uniform(model_loc_, r.get_entity().model(r.time));
    uniform(view_loc_, r.get_camera().view(r.time));
    uniform(proj_loc_, r.get_camera().projection());
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
	locate_uniforms();
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

void basic_shader_program::uniform(GLuint location, int value)
{
    glUniform1i(location, value);
}

void basic_shader_program::uniform(GLuint location, float value)
{
    glUniform1f(location, value);
}

void basic_shader_program::uniform(GLuint location, const glm::vec2& value)
{
    glUniform2fv(location, 1, glm::value_ptr(value));
}

void basic_shader_program::uniform(GLuint location, const glm::vec3& value)
{
    glUniform3fv(location, 1, glm::value_ptr(value));
}

void basic_shader_program::uniform(GLuint location, const glm::mat4& value)
{
    glUniformMatrix4fv(location, 1, GL_TRUE, glm::value_ptr(value));
}

void basic_shader_program::uniform(GLuint location, const std::vector<glm::mat4>& values)
{
    glUniformMatrix4fv(location, values.size(), GL_TRUE, reinterpret_cast<float const*>(values.data()));
}

void basic_shader_program::locate_uniforms()
{
	model_loc_ = locate_uniform("model");
	view_loc_ = locate_uniform("view");
	proj_loc_ = locate_uniform("projection");
}
