#include "Buffer.h"

Buffer::~Buffer()
{
	if (_buffer)
	{
		glDeleteBuffers(1, &_buffer);
	}
}

bool Buffer::Initialize(uint32_t target, uint32_t usage, const void* data, size_t stride, size_t count)
{
	_target = target;
	_usage = usage;
	_stride = stride;
	_count = count;
	
	glGenBuffers(1, &_buffer);
	Bind();
	glBufferData(target, stride * count, data, usage);

	/*
	glBind...() 함수를 이용하여 지금부터 사용할 object를 선택한다.

	// Vertex Buffer 생성
	glGenBuffers(1, &_vertexBuffer);
	// Vertex Buffer object 바인딩 
	// GL_ARRAY_BUFFER : Vertex data를 저장할 용도
	// Vertex Array Object가 사용할 Buffer -> _vertexBuffer
	glBindBuffer(GL_ARRAY_BUFFER, _vertexBuffer);
	// 바인딩된 buffer에 데이터 복사
	// 사이즈, 데이터의 포인터, 용도
	// GL_STATIC_DRAW : 변화가 없는 데이터
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 12, vertices, GL_STATIC_DRAW);
	*/

	/*
	// Element Buffer Object
	// 정점의 중복을 막는다.
	glGenBuffers(1, &_indexBuffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _indexBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint32_t) * 6, indices, GL_STATIC_DRAW);
	*/

    return true;
}

void Buffer::Bind() const
{
	glBindBuffer(_target, _buffer);
}

