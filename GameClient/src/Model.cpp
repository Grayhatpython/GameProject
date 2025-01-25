#include "Model.h"
#include "Mesh.h"
#include "Image.h"
#include "Texture.h"
#include <spdlog/spdlog.h>

std::unique_ptr<Model> Model::Load(const std::string& filename)
{
	auto model = std::unique_ptr<Model>(new Model());
	if (model->LoadByAssimp(filename) == false)
		return nullptr;
	return std::move(model);
}

void Model::Render(const Program* program) const
{
	for (auto& mesh : _meshes) {
		mesh->Render(program);
	}
}

bool Model::LoadByAssimp(const std::string& filename)
{
	Assimp::Importer importer;
	auto scene = importer.ReadFile(filename, aiProcess_Triangulate | aiProcess_FlipUVs);

	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) 
		return false;

	auto dirname = filename.substr(0, filename.find_last_of("/"));
	auto LoadTexture = [&](aiMaterial* material, aiTextureType type) -> std::shared_ptr<Texture> 
	{
		if (material->GetTextureCount(type) <= 0)
			return nullptr;
		aiString filepath;
		material->GetTexture(type, 0, &filepath);
		auto image = Image::Load(fmt::format("{}/{}", dirname, filepath.C_Str()));
		if (!image)
			return nullptr;
		return Texture::Create(image.get());
	};

	for (uint32_t i = 0; i < scene->mNumMaterials; i++) {
		auto material = scene->mMaterials[i];
		auto glMaterial = Material::Create();
		glMaterial->_diffuse = LoadTexture(material, aiTextureType_DIFFUSE);
		glMaterial->_specular = LoadTexture(material, aiTextureType_SPECULAR);
		_materials.push_back(std::move(glMaterial));
	}

	ProcessNode(scene->mRootNode, scene);
	return true;
}

void Model::ProcessMesh(aiMesh* mesh, const aiScene* scene)
{
	printf("process mesh: %s, #vert: %u, #face: %u\n", mesh->mName.C_Str(), mesh->mNumVertices, mesh->mNumFaces);

	std::vector<Vertex> vertices;
	vertices.resize(mesh->mNumVertices);
	for (uint32_t i = 0; i < mesh->mNumVertices; i++) 
	{
		auto& v = vertices[i];
		v.position = glm::vec3(mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z);
		v.normal = glm::vec3(mesh->mNormals[i].x, mesh->mNormals[i].y, mesh->mNormals[i].z);
		v.textureCoord = glm::vec2(mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y);
	}

	std::vector<uint32_t> indices;
	//	mNumFaces : Triangle 개수
	//	Triangle * 3 = index 개수
	indices.resize(mesh->mNumFaces * 3);
	for (uint32_t i = 0; i < mesh->mNumFaces; i++)
	{
		indices[3 * i] = mesh->mFaces[i].mIndices[0];
		indices[3 * i + 1] = mesh->mFaces[i].mIndices[1];
		indices[3 * i + 2] = mesh->mFaces[i].mIndices[2];
	}

	auto glMesh = Mesh::Create(vertices, indices, GL_TRIANGLES);
	if (mesh->mMaterialIndex >= 0)
		glMesh->SetMaterial(_materials[mesh->mMaterialIndex]);
	_meshes.push_back(std::move(glMesh));
}

void Model::ProcessNode(aiNode* node, const aiScene* scene)
{
	for (uint32_t i = 0; i < node->mNumMeshes; i++) 
	{
		auto meshIndex = node->mMeshes[i];
		auto mesh = scene->mMeshes[meshIndex];
		ProcessMesh(mesh, scene);
	}

	for (uint32_t i = 0; i < node->mNumChildren; i++) 
	{
		ProcessNode(node->mChildren[i], scene);
	}
}
