#include "bitmap_loader.hpp"
#include <algorithm>
#include "stb_image.h"

using namespace indigo;

bool bitmap_loader::can_load(const std::string& extension) const
{
    static char const* const exts[] = {
        "tga", "png", "bmp", "tif", "tiff"
    };

    return std::any_of(exts, exts + sizeof(exts)/sizeof(exts[0]), [&](char const* ext){
        return extension == ext;
    });
}

std::shared_ptr<bitmap> bitmap_loader::load(std::string const& filename)
{
    stbi_set_flip_vertically_on_load(true);

    int x(0), y(0), n(0);
    uint8_t* data = stbi_load(filename.c_str(), &x, &y, &n, 4);
    if (!data)
        return {nullptr};

    if (n != 4)
        throw std::runtime_error("Error loading image. Image data must have 4 channels!");

    return std::shared_ptr<bitmap>(new bitmap(x, y, data));
}
