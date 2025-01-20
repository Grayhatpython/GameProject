#include "Buffer.h"

Buffer::~Buffer()
{
	if (_buffer)
	{
		glDeleteBuffers(1, &_buffer);
	}
}

bool Buffer::Initialize(uint32_t target, uint32_t usage, const void* data, size_t dataSize)
{
	_target = target;
	_usage = usage;
	
	glGenBuffers(1, &_buffer);
	Bind();
	glBufferData(target, dataSize, data, usage);

	/*
	glBind...() �Լ��� �̿��Ͽ� ���ݺ��� ����� object�� �����Ѵ�.

	// Vertex Buffer ����
	glGenBuffers(1, &_vertexBuffer);
	// Vertex Buffer object ���ε� 
	// GL_ARRAY_BUFFER : Vertex data�� ������ �뵵
	// Vertex Array Object�� ����� Buffer -> _vertexBuffer
	glBindBuffer(GL_ARRAY_BUFFER, _vertexBuffer);
	// ���ε��� buffer�� ������ ����
	// ������, �������� ������, �뵵
	// GL_STATIC_DRAW : ��ȭ�� ���� ������
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 12, vertices, GL_STATIC_DRAW);
	*/

	/*
	// Element Buffer Object
	// ������ �ߺ��� ���´�.
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

std::unique_ptr<Buffer> Buffer::Create(uint32_t target, uint32_t usage, const void* data, size_t dataSize)
{
    auto buffer = std::unique_ptr<Buffer>(new Buffer());
    if (buffer->Initialize(target, usage, data, dataSize) == false)
        return nullptr;

    return std::move(buffer);
}
