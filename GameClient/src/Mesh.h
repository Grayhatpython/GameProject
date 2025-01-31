#pragma once

#include "VertexLayout.h"

class Buffer;
class Texture;
struct Vertex 
{
    glm::vec3 position;
    glm::vec3 normal;
    glm::vec2 textureCoord;
};

class Material
{
private:
    Material() = default;

public:
    void PushData(const std::shared_ptr<Program>& program);

public:
    std::shared_ptr<Texture>    _diffuse;
    std::shared_ptr<Texture>    _specular;
    float                       _shininess{ 32.0f };
};

class Mesh 
{
public:
    void Render() const;

public:
    void Initialize(const std::vector<Vertex>& vertices,const std::vector<uint32_t>& indices,uint32_t primitiveType);

public:
    const VertexLayout*         GetVertexLayout() const { return _vertexLayout.get(); }
    std::shared_ptr<Buffer>     GetVertexBuffer() const { return _vertexBuffer; }
    std::shared_ptr<Buffer>     GetIndexBuffer() const { return _indexBuffer; }

private:
    std::unique_ptr<VertexLayout>   _vertexLayout;
    std::shared_ptr<Buffer>         _vertexBuffer;
    std::shared_ptr<Buffer>         _indexBuffer;

    uint32_t                        _primitiveType{ GL_TRIANGLES };
};