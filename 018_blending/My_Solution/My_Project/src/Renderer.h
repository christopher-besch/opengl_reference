#pragma once

#include <GL/glew.h>

#include "VertexArray.h"
#include "IndexBuffer.h"
#include "Shader.h"

//                          <- compiler dependent
#define ASSERT(x) if (!(x)) __debugbreak();
#ifdef DEBUG
// #x turns x into a string
#define GLCALL(x) gl_clear_errors();\
	x;\
	ASSERT(gl_log_call(#x, __FILE__, __LINE__))
#else
#define GLCALL(x) x
#endif

void gl_clear_errors();
bool gl_log_call(const char* function, const char* file, int line);

void GLAPIENTRY error_occurred_gl(GLenum source,
    GLenum type,
    GLuint id,
    GLenum severity,
    GLsizei length,
    const GLchar * message,
    const void* userParam);


class Renderer
{
private:
	
public:
	void clear() const;
	void draw(const VertexArray& va, const IndexBuffer& ib, Shader& shader) const;
};
