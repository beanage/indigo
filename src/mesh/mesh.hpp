#pragma once

#include <vector>
#include <memory>
#include <glm/glm.hpp>
#include "platform/gl.hpp"

namespace indigo
{
class basic_shader_program;

enum class primitive : GLenum
{
	points         = GL_POINTS,
	lines          = GL_LINES,
	line_strip     = GL_LINE_STRIP,
	line_loop      = GL_LINE_LOOP,
	triangle_strip = GL_TRIANGLE_STRIP,
	triangle_fan   = GL_TRIANGLE_FAN,
	triangles      = GL_TRIANGLES,
};

class mesh
{
public:
	mesh(primitive p = primitive::triangles);

	mesh(const mesh& m);

	mesh(mesh&& m) noexcept;
	mesh& operator=(mesh&& m) noexcept;

	~mesh();

	void render(basic_shader_program& s);
	void upload();

	const std::vector<glm::vec3>& vertices() const;
	void vertices(const std::vector<glm::vec3>& v);

	const std::vector<glm::vec3>& normals() const;
	void normals(const std::vector<glm::vec3>& n);

	const std::vector<glm::vec3>& colors() const;
	void colors(const std::vector<glm::vec3>& c);

	const std::vector<glm::vec2>& texcoords() const;
	void texcoords(const std::vector<glm::vec2>& t);

	const std::vector<uint32_t>& indices() const;
	void indices(const std::vector<uint32_t>& i);

private:
	std::vector<glm::vec3> vertices_;
	std::vector<glm::vec3> normals_;
	std::vector<glm::vec3> colors_;
	std::vector<glm::vec2> texcoords_;
	std::vector<uint32_t> indices_;

	gl_handle vbo_;
	gl_handle ibo_;
	gl_handle vao_;
	primitive prim_;
};

mesh create_plane_z();
}
