#include "Renderer.h"

#include <iostream>


void gl_clear_errors()
{
	while (glGetError() != GL_NO_ERROR)
	{

	}
}


bool gl_log_call(const char* function, const char* file, int line)
{
	while (GLenum error = glGetError())
	{
		std::cout << "[OpenGL Error] (" << error << "): " << function << " " << file << ": line " << line << std::endl;
		return false;
	}
	return true;
}


void GLAPIENTRY error_occurred_gl(GLenum source,
	GLenum type,
	GLuint id,
	GLenum severity,
	GLsizei length,
	const GLchar* message,
	const void* userParam)
{
	printf("Message from OpenGL:\nSource: 0x%x\nType: 0x%x\n"
		"Id: 0x%x\nSeverity: 0x%x\n", source, type, id, severity);
	printf("%s\n", message);
	// shut down the program gracefully (it does cleanup stuff too)
	// without exit(), OpenGL will constantly print the error message
	// exit(-1);
	__debugbreak();
}


void Renderer::clear() const
{
	glClear(GL_COLOR_BUFFER_BIT);
}

//                                                                <- the shader can't be const <- a method of Shader used is not const
void Renderer::draw(const VertexArray& va, const IndexBuffer& ib, Shader& shader) const
{
	shader.bind();

	// glBindBuffer(GL_ARRAY_BUFFER, buffer);
	// structure of the vertex buffer has to be set every time
	// glEnableVertexAttribArray(0);
	// glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, (const void*)0);

	// bind vao and index buffer
	va.bind();
	ib.bind();

	//             <- mode
	//                           <- count = amount of indices being drawn
	//                              <- type of the data in the index buffer
	//                                               <- pointer to ibo, ibo is bound -> nullptr
	glDrawElements(GL_TRIANGLES, ib.get_count(), GL_UNSIGNED_INT, nullptr);
}
