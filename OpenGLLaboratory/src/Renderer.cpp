#include "Renderer.h"
#include <iostream>

/* ALl the Forward Declarations */
#include "Shader.h"
#include "VertexArray.h"
#include "IndexBuffer.h"

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

namespace olab {

	void Renderer::Draw(const VertexArray& _va, const IndexBuffer& _ib, const Shader& _shader) {

		_shader.use();
		_va.Bind();
		_ib.Bind();
		GLCall(glDrawElements(GL_TRIANGLES, _ib.Count(), GL_UNSIGNED_INT, nullptr));
	}

	void Renderer::Clear()
	{
		GLCall(glClear(GL_COLOR_BUFFER_BIT));
	}
}
