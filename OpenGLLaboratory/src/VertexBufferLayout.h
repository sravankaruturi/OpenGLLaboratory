#pragma once
#include <vector>
#include "Renderer.h"


struct VertexBufferElement {

	unsigned int type;
	unsigned int count;
	unsigned char normalized;

	static unsigned int GetSizeOfType(unsigned int _type) {

		switch (_type) { 

		case GL_FLOAT:			return 4;
		case GL_UNSIGNED_INT:	return 4;
		case GL_UNSIGNED_BYTE:	return 1;
		default:
			ASSERT(false);
			return 0;
		}
	}

};

class VertexBufferLayout {

private:
	std::vector<VertexBufferElement> elements;
	unsigned int stride;

public:
	VertexBufferLayout()
		: stride(0)
	{

	}

	const std::vector<VertexBufferElement>& GetElements() const
	{
		return elements;
	}

	template<typename T>
	void Push(unsigned int _count) {
		static_assert(false);
	}

	template<>
	void Push<float>(unsigned int _count) {
		elements.push_back({ GL_FLOAT,  _count, GL_FALSE });
		stride += _count * VertexBufferElement::GetSizeOfType(GL_FLOAT);
	}

	template<>
	void Push<unsigned int>(unsigned int _count) {
		elements.push_back({ GL_UNSIGNED_INT,  _count, GL_FALSE });
		stride += _count * VertexBufferElement::GetSizeOfType(GL_UNSIGNED_INT);
	}

	template<>
	void Push<unsigned char>(unsigned int _count) {
		elements.push_back({ GL_UNSIGNED_BYTE,  _count, GL_TRUE });
		stride += _count * VertexBufferElement::GetSizeOfType(GL_UNSIGNED_BYTE);
	}

	inline unsigned int GetStride() const { return stride; }

};