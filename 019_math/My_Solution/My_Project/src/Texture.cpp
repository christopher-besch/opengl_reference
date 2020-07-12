#include "Texture.h"

#include "stb_image/stb_image.h"

Texture::Texture(const std::string& path)
	: m_renderer_id(0), m_file_path(path), m_local_buffer(nullptr), m_width(0), m_height(0), m_bpp(0)
{
	// the texture has to be flipped <- OpenGL expects the start to be in the bottom whereas pngs start at the top
	stbi_set_flip_vertically_on_load(1);

	//                                                                    <- how many channels? 4 <- RGBA
	m_local_buffer = stbi_load(path.c_str(), &m_width, &m_height, &m_bpp, 4);

	glGenTextures(1, &m_renderer_id);
	glBindTexture(GL_TEXTURE_2D, m_renderer_id);

	// linear resample on minification filter
	//            <- type of data (integer)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	// magnification filter
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// wrap mode
	//                                             <- horizontal wrap
	//                                                <- clamp -> don't extend the area (no tiling)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	// same with vertical wrap
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	
	//                          <- not a multi-level texture
	//                             <- internal format -> how OpenGL stores texture data
	//                                    <- bits per chanel
	//                                                          <- border thickness
	//                                                             <- format -> format of the data OpenGL is provided with (m_local_buffer)
	//                                                                      <- type of the data
	//                                                                                        <- pointer to pixels (can be nullptr -> only create buffer and later on load pixels)
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, m_width, m_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, m_local_buffer);
	glBindTexture(GL_TEXTURE_2D, 0);

	// fre the memory after use
	if (m_local_buffer)
	{
		stbi_image_free(m_local_buffer);
	}
}


Texture::~Texture()
{
	glDeleteTextures(1, &m_renderer_id);
}


void Texture::bind(const unsigned int slot) const
{
	//                            <- texture slot (OpenGL supports up to 31 but the platform used might not)
	glActiveTexture(GL_TEXTURE0 + slot);
	glBindTexture(GL_TEXTURE_2D, m_renderer_id);
}


void Texture::unbind() const
{
	glBindTexture(GL_TEXTURE_2D, 0);
}
