#pragma once

#include "Renderer.h"


class Texture
{
private:
	unsigned int m_renderer_id;
	std::string m_file_path;
	unsigned char* m_local_buffer;
	//                     <- bytes per pixel
	int m_width, m_height, m_bpp;
public:
	Texture(const std::string& path);
	~Texture();

	void bind(const unsigned int slot) const;
	void unbind() const;

	inline int get_width() const
	{
		return m_width;
	}

	inline int get_height() const
	{
		return m_height;
	}
};