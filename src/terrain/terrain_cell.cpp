#include "terrain_cell.hpp"
#include "terrain.hpp"
#include "resource_manager.hpp"
#include "bitmap.hpp"
#include <glm/gtx/intersect.hpp>

using namespace indigo;

terrain_cell::terrain_cell(const terrain* parent, unsigned x, unsigned y)
    : vbo_(0), vao_(0), ibo_(0), terrain_(parent), x_(x), y_(y)
{
    generate_vertices();
    apply_heightmap(nullptr);
}

terrain_cell::~terrain_cell()
{
    glDeleteBuffers(1, &vbo_);
    glDeleteBuffers(1, &ibo_);
    glDeleteVertexArrays(1, &vao_);
}

void terrain_cell::render() const
{
    glBindVertexArray(vao_);
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);
    glDrawElements(GL_TRIANGLES, indices_.size(), GL_UNSIGNED_INT, (void*)0);
    glBindVertexArray(0);
}

void terrain_cell::upload()
{
    glGenBuffers(1, &vbo_);
    glGenBuffers(1, &ibo_);
    glGenVertexArrays(1, &vao_);

    glBindVertexArray(vao_);

    glBindBuffer(GL_ARRAY_BUFFER, vbo_);
    glBufferData(GL_ARRAY_BUFFER, vertices_.size() * sizeof(vertex),
                 vertices_.data(), GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo_);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices_.size() * sizeof(unsigned),
                 indices_.data(), GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(vertex), (void*)0);

    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(vertex),
                          (void*)sizeof(glm::vec3));

    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_TRUE, sizeof(vertex),
                          (void*)(sizeof(glm::vec3) * 2));

    glBindVertexArray(0);
}

bool terrain_cell::intersects(glm::vec3 from, glm::vec3 to, glm::mat4 model,
                              glm::vec3& position, unsigned& x, unsigned& y)
{
    glm::vec3 nearest_collision(-1, -1, -1);
    for (unsigned i = 0; i < indices_.size(); i += 3) {
        const int v0 = indices_[i + 2];
        const int v1 = indices_[i + 1];
        const int v2 = indices_[i + 0];

        glm::vec3 triangle[] = {vertices_[v0].pos, vertices_[v1].pos,
                                vertices_[v2].pos};

        if (glm::intersectRayTriangle(
                from, to, glm::vec3(glm::vec4(triangle[0], 1) * model),
                glm::vec3(glm::vec4(triangle[1], 1) * model),
                glm::vec3(glm::vec4(triangle[2], 1) * model), position)) {
            if (nearest_collision.z < 0.f || nearest_collision.z > position.z) {
                nearest_collision = position;

                x = i / 6 % size;
                y = i / 6 / size;
            }
        }
    }

    return nearest_collision.z >= 0.f;
}

unsigned terrain_cell::x() const
{
    return x_;
}

unsigned terrain_cell::y() const
{
    return y_;
}

void terrain_cell::generate_vertices()
{
    const unsigned cols = size + 1;
    const unsigned rows = size + 1;

    const float min_edge = 0.f;
    const float max_edge = size * tile_size;
    const float distance = max_edge - min_edge;

    for (unsigned y = 0; y < rows; ++y) {
        for (unsigned x = 0; x < cols; ++x) {
            vertex& vert = vertices_[y * (size + 1) + x];

            vert.pos.x = x * distance / size + min_edge;
            vert.pos.y = y * distance / size + min_edge;
            vert.pos.z = 0.f;

            vert.uv.x = 1.f / size * x;
            vert.uv.y = 1.f / size * y;
        }
    }
}

void terrain_cell::apply_heightmap(bitmap const* heightmap)
{
    const unsigned cols = size + 1;
    const unsigned rows = size + 1;

    for (unsigned y = 0; y < rows; ++y) {
        for (unsigned x = 0; x < cols; ++x) {
            vertex& vert = vertices_[y * (size + 1) + x];

            const glm::ivec2 heightmap_uv(x_ * size + x, y_ * size + y);

            if (heightmap) {
                auto pixel = heightmap->pixel(heightmap_uv.x, heightmap_uv.y);
                vert.pos.z =
                    (pixel.r() / 255.f) - (pixel.g() / 255.f); // Das ist komisch!
            } else {
                vert.pos.z = 0.f;
            }
        }
    }
}

void terrain_cell::apply_opacitymap(bitmap const* opaquemap)
{
    indices_.clear();
    indices_.reserve(size * size * 6);

    for (unsigned y = 0; y < size; ++y) {
        for (unsigned x = 0; x < size; ++x) {
            if (opaquemap) {
                const bool skip_tile =
                    opaquemap->pixel(x_ * size + x, y_ * size + y).r() > 0;
                if (skip_tile)
                    continue;
            }

            const bool top_right_to_bottom_left =
                (x < size / 2 && y >= size / 2) || (x >= size / 2 && y < size / 2);

            const unsigned curr_row = y * (size + 1);
            const unsigned next_row = (y + 1) * (size + 1);

            // B---A
            // | X |
            // C---D

            const int va = curr_row + x + 1;
            const int vb = curr_row + x;
            const int vc = next_row + x;
            const int vd = next_row + x + 1;

            if (top_right_to_bottom_left) { // test
                indices_.push_back(va);
                indices_.push_back(vb);
                indices_.push_back(vc);

                indices_.push_back(vc);
                indices_.push_back(vd);
                indices_.push_back(va);
            } else {
                indices_.push_back(vb);
                indices_.push_back(vc);
                indices_.push_back(vd);

                indices_.push_back(vd);
                indices_.push_back(va);
                indices_.push_back(vb);
            }
        }
    }
}

void terrain_cell::compute_normals()
{
    return;

    const unsigned cols = size + 1;
    const unsigned rows = size + 1;

    auto left_cell = x_ > 0 ? terrain_->cell(x_ - 1, y_) : nullptr;
    auto right_cell = x_ < size ? terrain_->cell(x_ + 1, y_) : nullptr;
    auto top_cell = y_ > 0 ? terrain_->cell(x_, y_ - 1) : nullptr;
    auto bottom_cell = y_ < size ? terrain_->cell(x_, y_ + 1) : nullptr;

    for (unsigned y = 0; y < rows; ++y) {
        const unsigned row_offset = y * size;

        for (unsigned x = 0; x < cols; ++x) {
            vertex& current_vert = vertices_[row_offset + x];
            auto height = current_vert.pos.z;

            vertex const& left_vert =
                x > 0 ? vertices_[row_offset + x - 1] : left_cell ? left_cell->vertices_[row_offset + size - 1] : vertices_[row_offset + x];

            vertex const& right_vert =
                x < size ? vertices_[row_offset + x + 1] : right_cell ? right_cell->vertices_[row_offset] : vertices_[row_offset + x];

            vertex const& top_vert =
                y > 0 ? vertices_[row_offset - size + x] : top_cell ? top_cell->vertices_[size * (size - 1) + x] : vertices_[row_offset + x];

            vertex const& bottom_vert = y < size ? vertices_[row_offset + size + x] : bottom_cell ? bottom_cell->vertices_[x] : vertices_[row_offset + x];

            // current_vert.normal = normal;
        }
    }
}
