#pragma once
#include "Component.h"

class Mesh;
class Material;

class MeshRenderer : public Component
{
public:
	MeshRenderer();
	virtual ~MeshRenderer();

public:
	void Render();

	//	TODO
	virtual void Update() override { Render(); }

public:
	void SetMesh(std::shared_ptr<Mesh> mesh) { _mesh = mesh; }
	void SetMaterial(std::shared_ptr<Material> material) { _material = material; }

private:
	std::shared_ptr<Mesh>		_mesh;
	std::shared_ptr<Material>	_material;
};