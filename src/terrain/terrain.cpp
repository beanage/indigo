#include "terrain.hpp"
#include "terrain_cell.hpp"
#include "shader/program.hpp"
#include "bitmap.hpp"

using namespace indigo;

terrain::terrain(unsigned width, unsigned height)
    : width_(width), height_(height)
{
    for (unsigned y = 0; y < height_; ++y)
        for (unsigned x = 0; x < width_; ++x)
            cells_.push_back(std::unique_ptr<terrain_cell>(new terrain_cell(this, x, y)));

    for (auto& cell : cells_)
        cell->compute_normals();
}

unsigned terrain::width() const
{
    return width_;
}

unsigned terrain::height() const
{
    return height_;
}

terrain_cell* terrain::cell(unsigned x, unsigned y)
{
    return cells_[y * width_ + x].get();
}

terrain_cell const* terrain::cell(unsigned x, unsigned y) const
{
    return cells_[y * width_ + x].get();
}

void terrain::render(program& prog) const
{
    prog.set("cell_size", (float)terrain_cell::size);
    prog.set("tile_size", terrain_cell::tile_size);
    prog.set("cell_count", glm::vec2(width_, height_));

    for (auto const& cell : cells_) {
        prog.set("cell_position", glm::vec2((float)cell->x(), (float)cell->y()));

        cell->render();
    }
}

void terrain::upload()
{
    for (auto& cell : cells_)
        cell->upload();
}

void terrain::heightmap(std::shared_ptr<bitmap> heightmap)
{
    if (heightmap->width() != width() * terrain_cell::size + 1 ||
        heightmap->height() != height() * terrain_cell::size + 1)
        throw bad_heightmap_size_exception();

    heightmap_.swap(heightmap);
    for (auto& cell : cells_)
        cell->apply_heightmap(heightmap_.get());
}

void terrain::opacitymap(std::shared_ptr<bitmap> opacitymap)
{
    if (opacitymap->width() != width() * terrain_cell::size ||
        opacitymap->height() != height() * terrain_cell::size)
        throw bad_opacitymap_size_exception();

    opacitymap_.swap(opacitymap);
    for (auto& cell : cells_)
        cell->apply_opacitymap(opacitymap_.get());
}
