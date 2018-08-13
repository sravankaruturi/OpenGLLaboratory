#include "IndexBuffer.h"
#include "Renderer.h"

namespace olab {

	IndexBuffer::IndexBuffer(const unsigned int * _data, const unsigned int _count)
		: count(_count)
	{
		ASSERT(sizeof(unsigned int) == sizeof(GLuint));
		GLCall(glGenBuffers(1, &rendererId));
		GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, rendererId));
		GLCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, _count * sizeof(unsigned int), _data, GL_STATIC_DRAW));
	}

	IndexBuffer::~IndexBuffer()
	{
		GLCall(glDeleteBuffers(1, &rendererId));
	}

	void IndexBuffer::Bind() const
	{
		GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, rendererId));
	}

	void IndexBuffer::UnBind()
	{
		GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
	}

}