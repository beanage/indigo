#ifndef __TERRAIN_HPP_INCLUDED__
#define __TERRAIN_HPP_INCLUDED__

#include <vector>
#include <memory>

#include "terrain_mesh.hpp"

namespace indigo
{
class bitmap;
class program;
class terrain_cell;

class terrain
{
public:
    terrain(unsigned width, unsigned height);
    ~terrain();

    unsigned width() const;
    unsigned height() const;

    terrain_cell* cell(unsigned x, unsigned y);
    terrain_cell const* cell(unsigned x, unsigned y) const;

    void render(program& prog) const;
    void upload();

    void update_height(std::shared_ptr<bitmap> heightmap);
    void update_tiles(std::shared_ptr<bitmap> opaquemap);

private:
    unsigned width_;
    unsigned height_;
    std::vector<std::unique_ptr<terrain_cell>> cells_;

    // std::vector<tileset> tilesets_;
    // texture material_map_;
    // texture normal_map_;

    std::shared_ptr<bitmap> heightmap_;
    std::shared_ptr<bitmap> opaquemap_;

    //std::vector<terrain_mesh> meshes_;
};
}

#endif
