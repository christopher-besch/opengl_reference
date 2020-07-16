#pragma once

#include "VertexBuffer.h"


class VertexBufferLayout;


// vao
class VertexArray
{
private:
	unsigned int m_renderer_id;
public:
	VertexArray();
	~VertexArray();

	void add_buffer(const VertexBuffer& vb, const VertexBufferLayout& layout);

	void bind() const;
	void unbind() const;
};
