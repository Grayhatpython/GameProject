#include "VertexLayout.h"

VertexLayout::~VertexLayout()
{
    if (_vertexArrayObject) {
        glDeleteVertexArrays(1, &_vertexArrayObject);
    }
}

void VertexLayout::Initialize()
{
    /*
    // Vertex Array Object 생성
	glGenVertexArrays(1, &_vertexArrayObject);
	// Vertex Array object 바인딩 
	glBindVertexArray(_vertexArrayObject);
    */

    glGenVertexArrays(1, &_vertexArrayObject);
    Bind();
}

std::unique_ptr<VertexLayout> VertexLayout::Create()
{
    auto vertexLayout = std::unique_ptr<VertexLayout>(new VertexLayout());
    vertexLayout->Initialize();
    return std::move(vertexLayout);
}

void VertexLayout::Bind() const
{
    glBindVertexArray(_vertexArrayObject);
}

void VertexLayout::EnableVertexAttribArray(uint32_t attribIndex, int count, uint32_t type, bool normalized, size_t stride, uint64_t offset) const
{
    glEnableVertexAttribArray(attribIndex);
    glVertexAttribPointer(attribIndex, count, type, normalized, static_cast<GLsizei>(stride), reinterpret_cast<const void*>(offset));

    /*
	glEnableVertexAttribArray(0);
	// 0 -> shader location = 0
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, 0);
    */
}

void VertexLayout::DisableVertexAttribArray(uint32_t attribIndex)
{
    glDisableVertexAttribArray(attribIndex);
}

