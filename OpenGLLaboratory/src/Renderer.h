#pragma once

#ifndef GLEW_STATIC
#define GLEW_STATIC
#include <GL/glew.h>
#endif

#ifndef ASSERT(x)
#define ASSERT(x) if(!(x)) __debugbreak()
#endif // !ASSERT(x)

#ifndef GLCall(x)
#define GLCall(x) GLClearError();\
	x;\
	ASSERT(GLLogCall(#x, __FILE__, __LINE__))
#endif // GLCall(x)

void GLClearError();
bool GLLogCall(const char * _function, const char * _file, int _line);
