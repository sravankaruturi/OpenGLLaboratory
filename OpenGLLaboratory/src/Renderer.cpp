#include "Renderer.h"
#include <iostream>

void GLClearError() {
	while (glGetError() != GL_NO_ERROR);
}

bool GLLogCall(const char * _function, const char * _file, int _line) {
	while (auto error = glGetError()) {
		std::cout << "Open GL Error" << error << " at " << _file << ":" << _line << std::endl;
		return false;
	}
	return true;
}