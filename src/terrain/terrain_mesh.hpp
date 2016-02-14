#ifndef __TERRAIN_MESH_HPP_INCLUDED__
#define __TERRAIN_MESH_HPP_INCLUDED__

#include <glm/glm.hpp>
#include <memory>

namespace indigo {
class mesh;

class terrain_object
{
public:
    terrain_object(std::shared_ptr<mesh> const& m);
    ~terrain_object();

    // position in tiles, starting from top left.
    glm::ivec2 tile() const;
    void tile(glm::ivec2 t);

    // size in tiles.
    glm::ivec2 size() const;
    void size(glm::ivec2 s);

    void render();
    void upload();

private:
    glm::ivec2 tile_;
    glm::ivec2 size_;
    std::shared_ptr<mesh> mesh_;
};
}

#endif
