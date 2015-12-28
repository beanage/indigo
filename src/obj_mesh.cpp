#include "obj_mesh.hpp"
#include "gl.hpp"

#include <stdexcept>
#include <algorithm>

using namespace indigo;

obj_mesh::obj_mesh()
	: vao_(0), vbo_(0)
{}

void obj_mesh::upload()
{
	if (glIsBuffer(vbo_))
		throw std::runtime_error("[obj_mesh::upload] mesh is already online!");

	struct vertex
	{
		glm::vec3 v;
		glm::vec2 vn;
		glm::vec2 vt;
	};

	std::vector<vertex> data;
	data.resize(vertices_.size());

	std::transform(vertices_.begin(), vertices_.end(), data.begin(), data.begin(),
			[&](const auto& val, auto& vert){ vert.v = val; return vert; });
	std::transform(uvs_.begin(), uvs_.end(), data.begin(), data.begin(),
			[&](const auto& val, auto& vert){ vert.vt = val; return vert; });
	std::transform(normals_.begin(), normals_.end(), data.begin(), data.begin(),
			[&](const auto& val, auto& vert){ vert.vn = val; return vert; });

	glGenBuffers(1, &vbo_);
	glGenVertexArrays(1, &vao_);

	glBindVertexArray(vao_);

	glBindBuffer(GL_ARRAY_BUFFER, vbo_);
	glBufferData(GL_ARRAY_BUFFER, data.size() * sizeof(vertex), data.data(), GL_STATIC_DRAW);

	glEnableVertexAttribArray(mesh::vertex_attribute_index);
	glVertexAttribPointer(mesh::vertex_attribute_index, 3, GL_FLOAT, GL_FALSE, sizeof(vertex), reinterpret_cast<void*>(0));

	glEnableVertexAttribArray(mesh::texcoord_attribute_index);
	glVertexAttribPointer(mesh::texcoord_attribute_index, 2, GL_FLOAT, GL_TRUE, sizeof(vertex), reinterpret_cast<void*>(sizeof(glm::vec3)*1));

	glBindVertexArray(0);
}

void obj_mesh::render()
{
	if (!glIsBuffer(vbo_))
		throw std::runtime_error("not a buffer!");

	glBindVertexArray(vao_);

	glEnableVertexAttribArray(mesh::vertex_attribute_index);
	glEnableVertexAttribArray(mesh::texcoord_attribute_index);
	glDrawArrays(GL_TRIANGLES, 0, vertices_.size());

	glBindVertexArray(0);
}
