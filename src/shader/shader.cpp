#include "shader.hpp"

#include <fstream>
#include <sstream>

using namespace indigo;

struct shader::impl
{
	GLuint handle;

	impl(const std::string& source, GLuint type)
	{
		handle = glCreateShader(type);
		if (!handle)
			; // Error

		const char* c_source = source.c_str();
		glShaderSource(handle, 1, static_cast<const GLchar**>(&c_source), NULL);

		glCompileShader(handle);

		GLint compilation_status = 0;
		glGetShaderiv(handle, GL_COMPILE_STATUS, &compilation_status);
		if (compilation_status == GL_FALSE) {
			GLint log_length = 0;
			glGetShaderiv(handle, GL_INFO_LOG_LENGTH, &log_length);

			GLchar* log_buffer = new GLchar[log_length + 1];
			glGetShaderInfoLog(handle, log_length, NULL, log_buffer);
			std::string log = "error compiling shader: ";
			log += log_buffer;
			delete[] log_buffer;

			glDeleteShader(handle);

            throw std::runtime_error(log);
		}
	}

    ~impl()
	{
		glDeleteShader(handle);
	}
};

shader::shader(const std::string& source, GLuint type)
	: shared(new impl(source, type))
{}

shader::~shader()
{}

GLuint shader::handle() const
{
	return shared->handle;
}

shader indigo::load_shader(const std::string& filename, GLuint type)
{
	std::ifstream file;
	file.open(filename, std::ios::in | std::ios::binary);
	if (!file.is_open())
		throw std::runtime_error("error loading shader from file.");

	std::stringstream buffer;
	buffer << file.rdbuf();
	
	return shader(buffer.str(), type);
}
