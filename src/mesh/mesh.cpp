#include "mesh.hpp"
#include "shader/basic_program.hpp"
#include "shader/shader.hpp"
#include <stdexcept>
#include <cstddef>

using namespace indigo;

namespace
{
struct vertex
{
	glm::vec3 vert;
	glm::vec3 normal;
	glm::vec3 color;
	glm::vec2 texcoord;
};
}

mesh::mesh(primitive prim)
	: vbo_(0), ibo_(0), vao_(0), prim_(prim)
{}

mesh::mesh(const mesh& m)
	: vbo_(0), ibo_(0), vao_(0), vertices_(m.vertices_), normals_(m.normals_), colors_(m.colors_), texcoords_(m.texcoords_), prim_(m.prim_)
{}

mesh::mesh(mesh&& m) noexcept
	: vbo_(0), ibo_(0), vao_(0), vertices_(m.vertices_), normals_(m.normals_), colors_(m.colors_), texcoords_(m.texcoords_), prim_(m.prim_)
{
	std::swap(m.vbo_, vbo_);
	std::swap(m.ibo_, ibo_);
	std::swap(m.vao_, vao_);
}

mesh& mesh::operator=(mesh&& m) noexcept
{
	std::swap(m.vbo_, vbo_);
	std::swap(m.ibo_, ibo_);
	std::swap(m.vao_, vao_);
	std::swap(m.prim_, prim_);
	std::swap(m.vertices_, vertices_);
	std::swap(m.normals_, normals_);
	std::swap(m.colors_, colors_);
	std::swap(m.texcoords_, texcoords_);
	std::swap(m.indices_, indices_);
	return *this;
}

mesh::~mesh()
{
	if (vbo_) glDeleteBuffers(1, &vbo_);
	if (ibo_) glDeleteBuffers(1, &ibo_);
	if (vao_) glDeleteVertexArrays(1, &vao_);
}

void mesh::render(basic_shader_program& s)
{
	if (!glIsBuffer(vbo_))
		throw std::runtime_error("Buffer not initialized!");

	glBindVertexArray(vao_);
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);
	glEnableVertexAttribArray(3);
	if (indices_.empty())
		glDrawArrays(GLenum(prim_), 0, vertices_.size());
	else
		glDrawElements(GLenum(prim_), indices_.size(), GL_UNSIGNED_INT, nullptr);
	glBindVertexArray(0);
}

void mesh::upload()
{
	const bool use_normals = normals_.size() == vertices_.size();
	const bool use_colors = colors_.size() == vertices_.size();
	const bool use_texcoords = texcoords_.size() == vertices_.size();
	const bool use_indices = !indices_.empty();

	std::vector<vertex> data;
	for (size_t i = 0; i < vertices_.size(); ++i) {
		data.push_back(vertex{
			vertices_[i],
			use_normals   ? normals_[i]   : glm::vec3(0),
			use_colors    ? colors_[i]    : glm::vec3(1, 1, 1),
			use_texcoords ? texcoords_[i] : glm::vec2(0)
		});
	}
	
	glGenBuffers(1, &vbo_);
	glGenVertexArrays(1, &vao_);
	glBindVertexArray(vao_);

	glBindBuffer(GL_ARRAY_BUFFER, vbo_);
	glBufferData(GL_ARRAY_BUFFER, data.size() * sizeof(vertex), data.data(), GL_STATIC_DRAW);

	if (use_indices) {
		glGenBuffers(1, &ibo_);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo_);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices_.size() * sizeof(GLuint), indices_.data(), GL_STATIC_DRAW);
	}

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(vertex), (void*)offsetof(vertex, vert));

	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(vertex), (void*)offsetof(vertex, normal));

	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(vertex), (void*)offsetof(vertex, color));

	glEnableVertexAttribArray(3);
	glVertexAttribPointer(3, 2, GL_FLOAT, GL_TRUE,  sizeof(vertex), (void*)offsetof(vertex, texcoord));

	glBindVertexArray(0);
}

const std::vector<glm::vec3>& mesh::vertices() const
{
	return vertices_;
}

void mesh::vertices(const std::vector<glm::vec3>& v)
{
	vertices_ = v;
}

const std::vector<glm::vec3>& mesh::normals() const
{
	return normals_;
}

void mesh::normals(const std::vector<glm::vec3>& n)
{
	normals_ = n;
}

const std::vector<glm::vec3>& mesh::colors() const
{
	return colors_;
}

void mesh::colors(const std::vector<glm::vec3>& c)
{
	colors_ = c;
}

const std::vector<glm::vec2>& mesh::texcoords() const
{
	return texcoords_;
}

void mesh::texcoords(const std::vector<glm::vec2>& t)
{
	texcoords_ = t;
}

const std::vector<GLuint>& mesh::indices() const
{
	return indices_;
}

void mesh::indices(const std::vector<GLuint>& i)
{
	indices_ = i;
}

// basic geometry constructors

indigo::mesh indigo::create_cube()
{
	static const std::vector<glm::vec3> vertices = {
		{-.5f, -.5f,  .5f},
		{ .5f, -.5f,  .5f},
		{-.5f,  .5f,  .5f},
		{ .5f,  .5f,  .5f},
		{-.5f, -.5f, -.5f},
		{ .5f, -.5f, -.5f},
		{-.5f,  .5f, -.5f},
		{ .5f,  .5f, -.5f},
	};

	static const std::vector<GLuint> indices = {
		0, 1, 2, // front
		1, 3, 2,
		6, 5, 4, // back
		6, 7, 5,
		4, 0, 6, // left
		0, 2, 6,
		5, 3, 1, // right
		7, 3, 5,
		2, 3, 7, // top
		7, 6, 2,
		5, 1, 0, // bottom
		0, 4, 5,
	};

	mesh cube(primitive::triangles);
	cube.vertices(vertices);
	cube.indices(indices);

	return cube;
}

indigo::mesh indigo::create_plane_z()
{
	static const std::vector<glm::vec3> vertices = {
		{-.5f, -.5f, 0.f},
		{ .5f, -.5f, 0.f},
		{-.5f,  .5f, 0.f},
		{-.5f,  .5f, 0.f},
		{ .5f, -.5f, 0.f},
		{ .5f,  .5f, 0.f},
	};

	mesh plane(primitive::triangles);
	plane.vertices(vertices);

	return plane;
}
