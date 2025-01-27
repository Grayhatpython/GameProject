#pragma once

class Buffer 
{
public:
    ~Buffer();

public:
    bool    Initialize(uint32_t target, uint32_t usage, const void* data, size_t stride, size_t count);
    void    Bind() const;

public:
    uint32_t    Get() const { return _buffer; }
    size_t      GetStride() const { return _stride; }
    size_t      GetCount() const { return _count; }

private:
    uint32_t _buffer{ 0 };  // VBO EBO ...
    uint32_t _target{ 0 };  // GL_ARRAY_BUFFER | GL_ELEMENT_ARRAY_BUFFER ...
    uint32_t _usage{ 0 };   // GL_STATIC_DRAW | ...
    size_t   _stride{ 0 };
    size_t   _count{ 0 };
};