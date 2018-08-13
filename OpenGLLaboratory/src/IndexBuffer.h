#pragma once

class IndexBuffer
{

private:
	unsigned int rendererId;
	unsigned int count;

public:
	explicit IndexBuffer(const int * _data, const unsigned int _count);
	~IndexBuffer();

	void Bind() const;
	static void UnBind();
	
	unsigned int Count() const { return count; }

};
