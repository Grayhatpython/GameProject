#pragma once

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

class Program;
class Mesh;
class Material;
class Model 
{
private:
    Model() = default;

public:
    static std::unique_ptr<Model> Load(const std::string& filename);
    void    Render(const Program* program) const;

private:
    bool LoadByAssimp(const std::string& filename);
    void ProcessMesh(aiMesh* mesh, const aiScene* scene);
    void ProcessNode(aiNode* node, const aiScene* scene);

public:
    std::shared_ptr<Mesh> GetMesh(int index) const { return _meshes[index]; }
    int GetMeshCount() const { return static_cast<int>(_meshes.size()); }

private: 
    std::vector<std::shared_ptr<Mesh>>      _meshes;
    std::vector<std::shared_ptr<Material>>  _materials;

};