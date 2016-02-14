#ifndef __TERRAIN_HPP_INCLUDED__
#define __TERRAIN_HPP_INCLUDED__

#include <vector>
#include <memory>

namespace indigo {
class bitmap;
class program;
class terrain_cell;

class bad_heightmap_size_exception : public std::exception
{
};
class bad_opacitymap_size_exception : public std::exception
{
};

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

    void heightmap(std::shared_ptr<bitmap> heightmap);
    void opacitymap(std::shared_ptr<bitmap> opacitymap);

private:
    unsigned width_;
    unsigned height_;
    std::vector<std::unique_ptr<terrain_cell>> cells_;

    // std::vector<tileset> tilesets_;
    // texture material_map_;
    // texture normal_map_;

    std::shared_ptr<bitmap> heightmap_;
    std::shared_ptr<bitmap> opacitymap_;
};
}

#endif
