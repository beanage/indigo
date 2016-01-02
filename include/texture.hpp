#ifndef __TEXTTURE_HPP_INCLUDED__
#define __TEXTTURE_HPP_INCLUDED__

#include <string>
#include "colors.hpp"
#include "gl.hpp"

namespace indigo
{

class texture
{
public:
    explicit texture(const std::string& file);
    texture(unsigned char* buffer, unsigned len);
    texture(int width, int height);
    texture(rgba_color const& solid_color, uint32_t width, uint32_t height);
    virtual ~texture();

    virtual void bind() const;

    unsigned int handle() const;

    uint32_t width() const;
    uint32_t height() const;

private:
    texture() = delete;
    texture(const texture&) = delete;

    uint32_t width_;
    uint32_t height_;
    gl_handle obj_;
};

class framebuffer
{
public:
    framebuffer(const texture& t);
    ~framebuffer();

    void bind();
    void unbind();

    unsigned int handle() const;

private:
    framebuffer() = delete;
    framebuffer(const framebuffer&) = delete;

    const texture& texture_;
    gl_handle fbo_;
    gl_handle rbo_;
};
}

#endif
