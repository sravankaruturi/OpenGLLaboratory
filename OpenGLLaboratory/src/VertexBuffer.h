#pragma once

namespace olab {

	class VertexBuffer
	{

	private:
		unsigned int rendererId;

	public:
		unsigned GetRendererId() const
		{
			return rendererId;
		}

		explicit VertexBuffer(const void * _data, const size_t _size);
		~VertexBuffer();

		void Bind() const;
		static void UnBind();

	};

}