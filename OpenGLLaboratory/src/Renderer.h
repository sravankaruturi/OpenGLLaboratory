﻿#pragma once

#ifndef GLEW_STATIC
#define GLEW_STATIC
#include <GL/glew.h>
#endif

#ifndef ASSERT
#define ASSERT(x) if(!(x)) __debugbreak()
#endif // !ASSERT(x)

#ifndef GLCall
#define GLCall(x) GLClearError();\
	x;\
	ASSERT(GLLogCall(#x, __FILE__, __LINE__))
#endif // GLCall(x)


void GLClearError();
bool GLLogCall(const char * _function, const char * _file, int _line);

namespace olab {

	class Shader;
	class VertexArray;
	class IndexBuffer;

	class Renderer {

	public:

		void Draw(const VertexArray& _va, const IndexBuffer& _ib, const Shader& _shader);

		static void Clear();

	};

}