#pragma once

#include "platform/gl.hpp"

#include <string>
#include <memory>

namespace indigo
{
class shader
{
public:
	enum class type : GLenum
	{
		vertex   = GL_VERTEX_SHADER,
		fragment = GL_FRAGMENT_SHADER,
	};

	explicit shader(type t);
	~shader();

	shader() = delete;
	shader(const shader&) = delete;

	GLuint id() const;

	void source(const std::string& source);
	std::string source() const;

	bool compile();

private:
	enum type type_;
	GLuint id_;
	std::string source_;
};
}
