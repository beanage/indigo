#include "mesh.hpp"
#include "gl.hpp"

indigo::mesh::mesh() : vao_(0), vbo_(0), num_verts(0)
{
}

void indigo::mesh::upload(std::vector<vertex> verts)
{
    num_verts = verts.size();

    glGenBuffers(1, &vbo_);
    glGenVertexArrays(1, &vao_);

    glBindVertexArray(vao_);
    glBindBuffer(GL_ARRAY_BUFFER, vbo_);
    glBufferData(GL_ARRAY_BUFFER, num_verts * sizeof(vertex), verts.data(), GL_STATIC_DRAW);

    glEnableVertexAttribArray(mesh::vertex_attribute_index);
    glVertexAttribPointer(mesh::vertex_attribute_index, 3, GL_FLOAT, GL_FALSE, sizeof(vertex), reinterpret_cast<void*>(0));

    glEnableVertexAttribArray(mesh::normal_attribute_index);
    glVertexAttribPointer(mesh::normal_attribute_index, 3, GL_FLOAT, GL_FALSE, sizeof(vertex), reinterpret_cast<void*>(sizeof(glm::vec3)));

    glEnableVertexAttribArray(mesh::texcoord_attribute_index);
    glVertexAttribPointer(mesh::texcoord_attribute_index, 2, GL_FLOAT, GL_TRUE, sizeof(vertex), reinterpret_cast<void*>(sizeof(glm::vec3)+sizeof(glm::vec3)));
    glBindVertexArray(0);
}

void indigo::mesh::render() const
{
    glBindVertexArray(vao_);
    glEnableVertexAttribArray(mesh::vertex_attribute_index);
    glEnableVertexAttribArray(mesh::texcoord_attribute_index);
    glEnableVertexAttribArray(mesh::normal_attribute_index);
    glDrawArrays(GL_TRIANGLES, 0, num_verts);
    glBindVertexArray(0);
}
