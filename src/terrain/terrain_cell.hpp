#ifndef __TERRAIN_CELL_HPP_INCLUDED__
#define __TERRAIN_CELL_HPP_INCLUDED__

#include "platform/gl.hpp"
#include <glm/glm.hpp>
#include <array>
#include <vector>

namespace indigo {
class bitmap;
class terrain;

class terrain_cell
{
public:
    static constexpr unsigned size = 8;
    static constexpr float tile_size = 1.f;

    terrain_cell(const terrain* parent, unsigned x, unsigned y);
    ~terrain_cell();

    void render() const;
    void upload();

    bool intersects(glm::vec3 from, glm::vec3 to, glm::mat4 model,
                    glm::vec3& position, unsigned& x, unsigned& y);

    unsigned x() const;
    unsigned y() const;

    void compute_normals();
    void apply_heightmap(bitmap const* heightmap);
    void apply_opacitymap(bitmap const* opaquemap);

private:
    void generate_vertices();

    terrain const* terrain_;
    unsigned x_;
    unsigned y_;

    struct vertex
    {
        glm::vec3 pos;
        glm::vec3 normal;
        glm::vec2 uv;
    };

    std::array<vertex, (size + 1) * (size + 1)> vertices_;
    std::vector<unsigned> indices_;
    // TODO: Add list of tiles to allow picking and non opaque tiles

    gl_handle vbo_;
    gl_handle vao_;
    gl_handle ibo_;
};
}

#endif
