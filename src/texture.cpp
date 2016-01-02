#include "texture.hpp"
#include "gl.hpp"

#include <stdexcept>
#include <array>

#define STB_IMAGE_IMPLEMENTATION
#define STBI_NO_PSD
#define STBI_NO_PIC
#include "stb_image.h"

using namespace indigo;

static const unsigned char CHECKERBOARD_BMP[] = {
	0x4d, 0x42, 0x00, 0x56, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x46, 0x00, 0x00, 0x00, 0x38, 
	0x00, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00, 0x01, 0x00, 0x20, 0x00, 0x03, 
	0x00, 0x00, 0x00, 0x10, 0x00, 0x00, 0x0b, 0x13, 0x00, 0x00, 0x0b, 0x13, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0x00, 
	0x00, 0x00, 0x00, 0xff, 0x00, 0x00, 0xff, 0xff, 0xff, 0x00, 0x00, 0xff, 0x00, 0x00, 0x00, 0xff, 
	0x00, 0x00, 0xff, 0xff, 0xff, 0x00
};

static unsigned gen_gl_texture(unsigned char* data, int width, int height)
{
	GLuint obj;
	glGenTextures(1, &obj);
	glBindTexture(GL_TEXTURE_2D, obj);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_GENERATE_MIPMAP, GL_FALSE); // TODO: :)

        if (data)
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);

	glBindTexture(GL_TEXTURE_2D, 0);

	return obj;
}

texture::texture(unsigned char* buffer, unsigned len)
	: obj_(0)
        , width_(0)
        , height_(0)
{
	stbi_set_flip_vertically_on_load(true);

	int x(0), y(0), n(0);
	unsigned char* data = stbi_load_from_memory(buffer, len, &x, &y, &n, 4);
	if (!data)
		throw std::runtime_error("[texture] error loading texture from buffer!");
	
	obj_ = gen_gl_texture(data, x, y);
	stbi_image_free(data);

    width_ = x;
    height_ = y;
}

texture::texture(const rgba_color &solid_color, uint32_t width, uint32_t height)
    : obj_(0)
    , width_(width)
    , height_(height)
{
    size_t size = width*height*4*sizeof(uint8_t);
    uint8_t data[size];
    for(size_t i = 0; i < size; ++i)
        data[i] = solid_color.bytes[i % 4];
    obj_ = gen_gl_texture(data, width, height);
}

texture::texture(const std::string& file)
	: obj_(0)
        , width_(0)
        , height_(0)
{
	stbi_set_flip_vertically_on_load(true);

	int x(0), y(0), n(0);
	unsigned char* data = stbi_load(file.c_str(), &x, &y, &n, 4);
	if (!data)
		throw std::runtime_error("[texture] error loading texture!");

	obj_ = gen_gl_texture(data, x, y);
	stbi_image_free(data);

        width_ = x;
        height_ = y;
}

texture::texture(int width, int height)
        : obj_(0)
        , width_(width)
        , height_(height)
{
        gen_gl_texture(nullptr, width, height);
}

texture::~texture()
{
	glDeleteTextures(1, &obj_);
}

void texture::bind() const
{
	glBindTexture(GL_TEXTURE_2D, obj_);
}

unsigned int texture::handle() const
{
        return obj_;
}

uint32_t texture::width() const
{
        return width_;
}

uint32_t texture::height() const
{
        return height_;
}

framebuffer::framebuffer(const texture &t)
    : texture_(t)
    , fbo_(0)
{
    texture_.bind();

    glGenFramebuffers(1, &fbo_);
    glBindFramebuffer(GL_FRAMEBUFFER, fbo_);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texture_.handle(), 0);

    glGenRenderbuffers(1, &rbo_);
    glBindRenderbuffer(GL_RENDERBUFFER, rbo_);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, texture_.width(), texture_.height());

    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, rbo_);

    GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
    if (status != GL_FRAMEBUFFER_COMPLETE)
        throw std::runtime_error("[framebuffer] error creating fbo!");

    unbind();
}

framebuffer::~framebuffer()
{
    unbind();
    glDeleteFramebuffers(1, &fbo_);
    glDeleteRenderbuffers(1, &rbo_);
}

void framebuffer::bind()
{
    glBindFramebuffer(GL_FRAMEBUFFER, fbo_);
}

void framebuffer::unbind()
{
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glBindRenderbuffer(GL_RENDERBUFFER, 0);
}

unsigned int framebuffer::handle() const
{
    return fbo_;
}
