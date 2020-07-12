#pragma once

#include <vector>

#include "Renderer.h"


// one vertex buffer attribute
struct VertexBufferElement
{
	// OpenGL type
	unsigned int type;
	unsigned int count;
	unsigned char normalized;

	static unsigned int get_size_of_type(unsigned int type)
	{
		switch (type)
		{
			case GL_FLOAT:         return 4;
			case GL_UNSIGNED_INT:  return 4;
			case GL_UNSIGNED_BYTE: return 1;
		}
		ASSERT(false);
		return 0;
	}
};


// all vertex buffer attributes for one vertex buffer
class VertexBufferLayout
{
private:
	std::vector<VertexBufferElement> m_elements;
	unsigned int m_stride;
public:
	VertexBufferLayout()
		: m_stride(0) {}

	template<typename T>
	void push(unsigned int count)
	{
		// when the type is unmatched
		ASSERT(false);
		
		// this seems to create problems with gcc
		static_assert(false);
	}

	/////////////////////////////
	// appears to be msvc only //
	/////////////////////////////
	
	// template<>
	// void push<float>(unsigned int count)
	// {
		//                                     <- hard coded for now
		// m_elements.push_back({ GL_FLOAT, count, GL_FALSE });
		// m_stride += count * VertexBufferElement::get_size_of_type(GL_FLOAT);
	// }

	// template<>
	// void push<unsigned int>(unsigned int count)
	// {
		// m_elements.push_back({ GL_UNSIGNED_INT, count, GL_FALSE });
		// m_stride += count * VertexBufferElement::get_size_of_type(GL_UNSIGNED_INT);
	// }

	// template<>
	// void push<unsigned char>(unsigned int count)
	// {
		// m_elements.push_back({ GL_UNSIGNED_BYTE, count, GL_TRUE });
		// m_stride += count * VertexBufferElement::get_size_of_type(GL_UNSIGNED_BYTE);
	// }

	inline const std::vector<VertexBufferElement>& get_elements() const
	{
		return m_elements;
	}
	
	inline unsigned int get_stride() const
	{
		return m_stride;
	}
};


template<>
inline void VertexBufferLayout::push<float>(unsigned int count)
{
	//                                     <- hard coded for now
	m_elements.push_back({ GL_FLOAT, count, GL_FALSE });
	m_stride += count * VertexBufferElement::get_size_of_type(GL_FLOAT);
}

template<>
inline void VertexBufferLayout::push<unsigned int>(unsigned int count)
{
	m_elements.push_back({ GL_UNSIGNED_INT, count, GL_FALSE });
	m_stride += count * VertexBufferElement::get_size_of_type(GL_UNSIGNED_INT);
}

template<>
inline void VertexBufferLayout::push<unsigned char>(unsigned int count)
{
	m_elements.push_back({ GL_UNSIGNED_BYTE, count, GL_TRUE });
	m_stride += count * VertexBufferElement::get_size_of_type(GL_UNSIGNED_BYTE);
}
