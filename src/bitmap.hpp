#ifndef __BITMAP_HPP_INCLUDED__
#define __BITMAP_HPP_INCLUDED__

#include <vector>
#include <inttypes.h>
#include "colors.hpp"

namespace indigo {
class bitmap
{
public:
    bitmap(uint32_t width, uint32_t height, rgba_color color);
    bitmap(uint32_t width, uint32_t height, uint8_t* rgba_data);

    bitmap(bitmap const& source) = default;
    bitmap(bitmap&& source) = default;
    ~bitmap() = default;

    uint32_t width() const noexcept;
    uint32_t height() const noexcept;

    rgba_color pixel(uint32_t x, uint32_t y) const;
    void pixel(uint32_t x, uint32_t y, rgba_color color);

private:
    bitmap() = delete;

    uint32_t width_;
    uint32_t height_;
    std::vector<uint8_t> buffer_;
};
}

#endif
