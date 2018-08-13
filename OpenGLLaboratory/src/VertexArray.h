#pragma once

namespace olab {

	class VertexBuffer;
	class VertexBufferLayout;

	class VertexArray
	{

	private:

		unsigned int rendererId;

	public:

		VertexArray();
		~VertexArray();

		void AddBuffer(const VertexBuffer& _vb, const VertexBufferLayout& _layout);

		void Bind() const;
		static void UnBind();

	};

}