#include "VertexBuffer.h"
#include "VertexBufferLayout.h"

#include "Renderer.h"

VertexBuffer::VertexBuffer(const void* data, unsigned size)
{
    // create vertex buffer and its id
    glGenBuffers(1, &m_renderer_id);
    // selecting the buffer with its id
    glBindBuffer(GL_ARRAY_BUFFER, m_renderer_id);
    //                            <- size of the buffer in bytes
    //                                  <- source
    //                                        <- STREAM -> modified once, used few times; STATIC -> modified once, used often, DYNAMIC -> modified repeatedly, used often
    glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
}

VertexBuffer::~VertexBuffer()
{
    glDeleteBuffers(1, &m_renderer_id);
}

void VertexBuffer::bind() const
{
    glBindBuffer(GL_ARRAY_BUFFER, m_renderer_id);
}

void VertexBuffer::unbind() const
{
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}
