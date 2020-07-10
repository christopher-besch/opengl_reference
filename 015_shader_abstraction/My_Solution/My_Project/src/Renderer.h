#pragma once

#include <GL/glew.h>

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
