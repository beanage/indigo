#include "bitmap.hpp"

#include <stdexcept>

using namespace indigo;

static const size_t bytes_per_pixel = 4;

static void check_bounds_and_throw(bitmap const& b, uint32_t x, uint32_t y)
{
    if (x >= b.width() || y >= b.height())
        throw std::runtime_error("Pixel position out of bounds!");
}

bitmap::bitmap(uint32_t width, uint32_t height, rgba_color color)
    : width_(width), height_(height), buffer_(width * height * bytes_per_pixel)
{
    for (size_t i = 0; i < width * height * bytes_per_pixel; ++i)
        buffer_[i] = color.bytes[i % bytes_per_pixel];
}

bitmap::bitmap(uint32_t width, uint32_t height, uint8_t* rgba_data)
    : width_(width), height_(height), buffer_(rgba_data, rgba_data + width * height * bytes_per_pixel)
{
}

uint32_t bitmap::width() const noexcept
{
    return width_;
}

uint32_t bitmap::height() const noexcept
{
    return height_;
}

rgba_color bitmap::pixel(uint32_t x, uint32_t y) const
{
    check_bounds_and_throw(*this, x, y);

    y *= bytes_per_pixel * width_;
    x *= bytes_per_pixel;

    return {buffer_[y + x + 0], buffer_[y + x + 1], buffer_[y + x + 2],
            buffer_[y + x + 3]};
}

void bitmap::pixel(uint32_t x, uint32_t y, rgba_color color)
{
    check_bounds_and_throw(*this, x, y);

    y *= bytes_per_pixel * width_;
    x *= bytes_per_pixel;

    buffer_[y + x + 0] = color.r();
    buffer_[y + x + 1] = color.g();
    buffer_[y + x + 2] = color.b();
    buffer_[y + x + 3] = color.a();
}
