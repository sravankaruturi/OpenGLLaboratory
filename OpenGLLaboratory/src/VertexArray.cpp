#include "VertexArray.h"
#include "Renderer.h"

#include "VertexBuffer.h"
#include "VertexBufferLayout.h"

namespace olab {

	VertexArray::VertexArray()
	{
		GLCall(glGenVertexArrays(1, &rendererId));
	}

	VertexArray::~VertexArray()
	{
		GLCall(glDeleteVertexArrays(1, &rendererId));
	}

	void VertexArray::AddBuffer(const VertexBuffer& _vb, const VertexBufferLayout& _layout)
	{
		Bind();
		_vb.Bind();
		const auto& elements = _layout.GetElements();

		int i = 0;
		unsigned int offset = 0;
		for (auto element : elements) {
			GLCall(glEnableVertexAttribArray(i));

			if (element.type == GL_UNSIGNED_INT || element.type == GL_INT) {
				GLCall(glVertexAttribIPointer(i, element.count, element.type, _layout.GetStride(), (const void *)offset));
			}
			else {
				// Check the Element Type. If it is float use the usual one. If it is int, use the new one.
				GLCall(glVertexAttribPointer(i, element.count, element.type, element.normalized, _layout.GetStride(), (const void *)offset));
			}
			offset += (element.count * VertexBufferElement::GetSizeOfType(element.type));
			i++;
		}

	}

	void VertexArray::Bind() const
	{
		GLCall(glBindVertexArray(rendererId));
	}

	void VertexArray::UnBind()
	{
		GLCall(glBindVertexArray(0));
	}

}