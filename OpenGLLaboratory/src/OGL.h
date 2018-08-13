#pragma once

#ifndef GLEW_STATIC
#define GLEW_STATIC
#include <GL/glew.h>
#endif

#include <iostream>

#ifndef ASSERT(x)
#define ASSERT(x) if(!(x)) __debugbreak()
#endif // !ASSERT(x)

#ifndef GLCall(x)
#define GLCall(x) GLClearError();\
	x;\
	ASSERT(GLLogCall(#x, __FILE__, __LINE__))
#endif // GLCall(x)

static void GLClearError() {
	while (glGetError() != GL_NO_ERROR);
}

static bool GLLogCall(const char * _function, const char * _file, int _line) {
	while (auto error = glGetError()) {
		std::cout << "Open GL Error" << error << " at " << _file << ":" << _line << std::endl;
		return false;
	}
	return true;
}