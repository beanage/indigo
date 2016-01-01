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
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
	glBindTexture(GL_TEXTURE_2D, 0);

	return obj;
}

texture::texture(unsigned char* buffer, unsigned len)
	: obj_(0)
{
	stbi_set_flip_vertically_on_load(true);

	int x(0), y(0), n(0);
	unsigned char* data = stbi_load_from_memory(buffer, len, &x, &y, &n, 4);
	if (!data)
		throw std::runtime_error("[texture] error loading texture from buffer!");
	
	obj_ = gen_gl_texture(data, x, y);
    stbi_image_free(data);
}

texture::texture(const rgba_color &solid_color, uint32_t width, uint32_t height)
{
    size_t size = width*height*4*sizeof(uint8_t);
    uint8_t data[size];
    for(size_t i = 0; i < size; ++i)
        data[i] = solid_color.bytes[i % 4];
    obj_ = gen_gl_texture(data, width, height);
}

texture::texture(const std::string& file)
	: obj_(0)
{
	stbi_set_flip_vertically_on_load(true);

	int x(0), y(0), n(0);
	unsigned char* data = stbi_load(file.c_str(), &x, &y, &n, 4);
	if (!data)
		throw std::runtime_error("[texture] error loading texture!");

	obj_ = gen_gl_texture(data, x, y);
	stbi_image_free(data);
}

texture::~texture()
{
	glDeleteTextures(1, &obj_);
}

void texture::bind() const
{
	glBindTexture(GL_TEXTURE_2D, obj_);
}
