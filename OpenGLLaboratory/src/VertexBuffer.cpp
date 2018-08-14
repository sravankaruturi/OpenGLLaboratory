#include "VertexBuffer.h"
#include "Renderer.h"

namespace olab {

	VertexBuffer::VertexBuffer(const void * _data, unsigned int _size)
	{
		GLCall(glGenBuffers(1, &rendererId));
		GLCall(glBindBuffer(GL_ARRAY_BUFFER, rendererId));
		GLCall(glBufferData(GL_ARRAY_BUFFER, _size, _data, GL_STATIC_DRAW));
	}

	VertexBuffer::~VertexBuffer()
	{
		GLCall(glDeleteBuffers(1, &rendererId));
	}

	void VertexBuffer::Bind() const
	{
		GLCall(glBindBuffer(GL_ARRAY_BUFFER, rendererId));
	}

	void VertexBuffer::UnBind()
	{
		GLCall(glBindBuffer(GL_ARRAY_BUFFER, 0));
	}

}