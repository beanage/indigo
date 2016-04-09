#include "shader.hpp"
#include "util/log.hpp"

#include <fstream>
#include <sstream>

using namespace indigo;

shader::shader(enum type t)
	: type_(t), id_(0u)
{
	id_ = glCreateShader(GLenum(type_));
}

shader::~shader()
{
	if (id_)
		glDeleteShader(id_);
}

GLuint shader::id() const
{
	return id_;
}

void shader::source(const std::string& source)
{
	source_ = source;
}

std::string shader::source() const
{
	return source_;
}

bool shader::compile()
{
	const GLchar* source(source_.c_str());

	glShaderSource(id_, 1, &source, NULL);
	glCompileShader(id_);

	GLint status(0);
	glGetShaderiv(id_, GL_COMPILE_STATUS, &status);

	GLint log_len(0);
	glGetShaderiv(id_, GL_INFO_LOG_LENGTH, &log_len);

	std::string log(log_len, '\0');
	glGetShaderInfoLog(id_, log.size(), nullptr, &log[0]);

	if (!status)
		log::write("Shader: ", log);

	return !status;
}

