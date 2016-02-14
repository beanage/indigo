#ifndef __INDIGO_COLORS_HPP_INCLUDED__
#define __INDIGO_COLORS_HPP_INCLUDED__

#include <stdint.h>

namespace indigo {
struct rgba_color
{
    rgba_color(uint8_t r, uint8_t g, uint8_t b, uint8_t a = 0xff)
    {
        bytes[0] = r, bytes[1] = g, bytes[2] = b, bytes[3] = a;
    }

    uint8_t bytes[4];

    inline uint8_t r()
    {
        return bytes[0];
    }
    inline uint8_t g()
    {
        return bytes[1];
    }
    inline uint8_t b()
    {
        return bytes[2];
    }
    inline uint8_t a()
    {
        return bytes[3];
    }
};

// define some colors
namespace colors {
static const rgba_color black(0x0, 0x0, 0x0);
static const rgba_color light_grey(0x40, 0x40, 0x40);
static const rgba_color red(0xFF, 0, 0);
static const rgba_color light_blue(0x66, 0x66, 0xFF);
static const rgba_color blue(0x33, 0x00, 0xCC);
static const rgba_color yellow(0xFF, 0xFF, 0x00);
static const rgba_color green(0x00, 0x99, 0x00);
static const rgba_color light_green(0x33, 0xCC, 0x33);
static const rgba_color purple(0xFF, 0x00, 0x66);
static const rgba_color violet(0x99, 0x33, 0x99);
static const rgba_color orange(0xFF, 0x99, 0x00);
static const rgba_color indigo(0x00, 0x99, 0x66);
}
}

#endif
