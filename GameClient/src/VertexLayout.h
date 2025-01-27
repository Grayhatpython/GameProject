#pragma once

class VertexLayout 
{ 
public:
    ~VertexLayout();

public:
    void Initialize();

public:
    void        Bind() const;
    void        EnableVertexAttribArray(uint32_t attribIndex, int count,uint32_t type, bool normalized,size_t stride, uint64_t offset) const;
    void        DisableVertexAttribArray(uint32_t attribIndex);

public:
    uint32_t    GetVertexArrayObject() const { return _vertexArrayObject; }


private:
    uint32_t    _vertexArrayObject{ 0 };
};