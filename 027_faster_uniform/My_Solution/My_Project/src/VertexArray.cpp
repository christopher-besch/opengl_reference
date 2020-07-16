#include "VertexArray.h"
#include "Renderer.h"
#include "VertexBufferLayout.h"

VertexArray::VertexArray()
{
	glGenVertexArrays(1, &m_renderer_id);
}

VertexArray::~VertexArray()
{
	glDeleteVertexArrays(1, &m_renderer_id);
}

void VertexArray::add_buffer(const VertexBuffer& vb, const VertexBufferLayout& layout)
{
	// bind the vao and the vertex buffer -> they get "connected"
	bind();
	vb.bind();
	unsigned int offset = 0;
	const auto& elements = layout.get_elements();
	// going through every element (=attribute)
	for (int i = 0; i < elements.size(); i++)
	{
		const auto& element = elements[i];
		// define the layout of the vertex buffer (the data)
		glEnableVertexAttribArray(i);
		// this connects the VAO with the vertex buffer
		// each vertex can have multiple attributes, one of which can be the position
		// define the layout of the data
		// defining 1 attribute, the position
		//                    <- index: index of this attribute in the each vertex, first attribute, second, third...
		//                       <- size: how many values are representing this attribute; 1, 2, 3, 4 are possible
		//                                      <- type of the values in this attribute
		//                                                    <- normalized: when true converts int (0-255) to float (0.0f - 1.0f)
		//                                                                        <- stride: amount of bytes between each vertex start
		//                                                                                             <- pointer: inside the space of a vertex, the start of this attribute, first attribute -> 0, second attribute -> >0
	    glVertexAttribPointer(i, element.count, element.type, element.normalized, layout.get_stride(), (const void*)offset);
		offset += element.count * VertexBufferElement::get_size_of_type(element.type);
	}
}

void VertexArray::bind() const
{
	glBindVertexArray(m_renderer_id);
}

void VertexArray::unbind() const
{
	glBindVertexArray(0);
}
