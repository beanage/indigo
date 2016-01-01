#ifndef __TEXTTURE_HPP_INCLUDED__
#define __TEXTTURE_HPP_INCLUDED__

#include <string>

#include "colors.hpp"

namespace indigo
{

class texture
{
public:
    explicit texture(const std::string& file);
    texture(unsigned char* buffer, unsigned len);
    texture(rgba_color const& solid_color, uint32_t width, uint32_t height);

    virtual ~texture();

    virtual void bind() const;

private:
    texture() = delete;
    texture(const texture&) = delete;

    unsigned int obj_;
};
}

#endif
