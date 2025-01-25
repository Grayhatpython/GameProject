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

class Program;
class Material
{
private:
    Material() = default;

public:
    static std::unique_ptr<Material> Create() { return std::unique_ptr<Material>(new Material()); }

public:
    void SetProgram(const Program* program) const;

public:
    std::shared_ptr<Texture>    _diffuse;
    std::shared_ptr<Texture>    _specular;
    float                       _shininess{ 32.0f };
};

class Mesh 
{
private:
    Mesh() = default;

public:
    static std::unique_ptr<Mesh> Create(const std::vector<Vertex>& vertices, const std::vector<uint32_t>& indices,uint32_t primitiveType);
    static std::unique_ptr<Mesh> CreateBox();

    void Render(const Program* program) const;

private:
    void Initialize(const std::vector<Vertex>& vertices,const std::vector<uint32_t>& indices,uint32_t primitiveType);

public:
    void                        SetMaterial(std::shared_ptr<Material> material) { _material = material; }
    std::shared_ptr<Material>   GetMaterial() const { return _material; }

    const VertexLayout*     GetVertexLayout() const { return _vertexLayout.get(); }
    std::shared_ptr<Buffer> GetVertexBuffer() const { return _vertexBuffer; }
    std::shared_ptr<Buffer> GetIndexBuffer() const { return _indexBuffer; }

private:
    std::shared_ptr<Material>       _material;

    std::unique_ptr<VertexLayout>   _vertexLayout;
    std::shared_ptr<Buffer>         _vertexBuffer;
    std::shared_ptr<Buffer>         _indexBuffer;

    uint32_t                        _primitiveType{ GL_TRIANGLES };
};