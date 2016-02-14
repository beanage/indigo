#include "md5_mesh.hpp"

using namespace indigo;

static const unsigned int texcoord_attribute_index   = 0;
static const unsigned int bone_start_attribute_index = 1;
static const unsigned int bone_count_attribute_index = 2;

md5_mesh::md5_mesh()
    : num_verts_(0)
{

}

md5_mesh::~md5_mesh()
{
    glDeleteBuffers(1, &vbo_);
    glDeleteVertexArrays(1, &vao_);
}

void md5_mesh::render() const
{
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_BUFFER, tbo_tex_);
    glTexBuffer(GL_TEXTURE_BUFFER, GL_R32F, tbo_);

    glBindVertexArray(vao_);
    glEnableVertexAttribArray(texcoord_attribute_index);
    glEnableVertexAttribArray(bone_start_attribute_index);
    glEnableVertexAttribArray(bone_count_attribute_index);
    glDrawArrays(GL_TRIANGLES, 0, num_verts_);
    glBindVertexArray(0);
}

void md5_mesh::upload()
{
    upload_vertices();
    upload_weights();
}

void md5_mesh::upload_vertices()
{
    num_verts_ = vertices_.size();

    glGenBuffers(1, &vbo_);
    glGenVertexArrays(1, &vao_);

    glBindVertexArray(vao_);
    glBindBuffer(GL_ARRAY_BUFFER, vbo_);
    glBufferData(GL_ARRAY_BUFFER, num_verts_ * sizeof(vertex), vertices_.data(), GL_STATIC_DRAW);

    glEnableVertexAttribArray(texcoord_attribute_index);
    glVertexAttribPointer(texcoord_attribute_index, 2, GL_FLOAT, GL_FALSE, sizeof(vertex), reinterpret_cast<void*>(0));

    glEnableVertexAttribArray(bone_start_attribute_index);
    glVertexAttribPointer(bone_start_attribute_index, 1, GL_FLOAT, GL_FALSE, sizeof(vertex), reinterpret_cast<void*>(sizeof(glm::vec2)));

    glEnableVertexAttribArray(bone_count_attribute_index);
    glVertexAttribPointer(bone_count_attribute_index, 1, GL_FLOAT, GL_FALSE, sizeof(vertex), reinterpret_cast<void*>(sizeof(glm::vec2)+sizeof(float)));
    glBindVertexArray(0);
}

void md5_mesh::upload_weights()
{
    num_weights_ = weights_.size();

    glGenBuffers(1, &tbo_);
    glBindBuffer(GL_TEXTURE_BUFFER, tbo_);
    glBufferData(GL_TEXTURE_BUFFER, sizeof(weight) * weights_.size(), weights_.data(), GL_STATIC_DRAW);
    glGenTextures(1, &tbo_tex_);
    glBindBuffer(GL_TEXTURE_BUFFER, 0);
}
