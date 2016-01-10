#include "program.hpp"

#include <glm/gtc/type_ptr.hpp>

using namespace indigo;

program::program(const std::vector<shader>& shaders)
{
	handle = glCreateProgram();
	if (!handle)
		; // Error

	std::vector<shader>::const_iterator shader_iter = shaders.begin();
	for (; shader_iter != shaders.end(); ++shader_iter)
		glAttachShader(handle, (*shader_iter).handle());

	glLinkProgram(handle);

	shader_iter = shaders.begin();
	for (; shader_iter != shaders.end(); ++shader_iter)
		glDetachShader(handle, (*shader_iter).handle());

	GLint link_status = 0;
	glGetProgramiv(handle, GL_LINK_STATUS, &link_status);
	if (link_status == GL_FALSE) {
		GLint log_length = 0;
		glGetProgramiv(handle, GL_INFO_LOG_LENGTH, &log_length);

		GLchar* log_buffer = new GLchar[log_length + 1];
		glGetProgramInfoLog(handle, log_length, NULL, log_buffer);
		std::string log = "error linking program: ";
		log += log_buffer;
		delete[] log_buffer;
		glDeleteProgram(handle);

		throw std::runtime_error(log);
	}
}

program::~program()
{
	glDeleteProgram(handle);
}

void program::use() const
{
	glUseProgram(handle);
}

GLint program::attribute(const std::string& name) const
{
	GLint temp = glGetAttribLocation(handle, name.c_str());
	if (temp == -1)
		throw std::runtime_error("invalid attribute name '" + name + "'!");

	return temp;
}

GLint program::uniform(const std::string& name) const
{
	GLint temp = glGetUniformLocation(handle, name.c_str());
	if (temp == -1)
		throw std::runtime_error("invalid uniform name '" + name + "'!");

	return temp;
}

void program::set(const std::string& name, GLuint value)
{
	glUniform1uiv(uniform(name), 1, &value);
}

void program::set(const std::string& name, const glm::mat4& value)
{
	glUniformMatrix4fv(uniform(name), 1, GL_TRUE, glm::value_ptr(value));
}

void program::set(const std::string& name, const glm::vec3& value)
{
    glUniform3fv(uniform(name), 1, glm::value_ptr(value));
}

void program::set(const std::string& name, const std::vector<glm::mat4>& values)
{
    glUniformMatrix4fv(uniform(name), values.size(), GL_TRUE, reinterpret_cast<float const*>(values.data()));
}
