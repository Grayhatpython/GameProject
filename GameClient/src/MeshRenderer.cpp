#include "MeshRenderer.h"
#include "Mesh.h"
#include "Transform.h"

MeshRenderer::MeshRenderer()
	: Component(ComponentType::MeshRenderer)
{

}

MeshRenderer::~MeshRenderer()
{

}

void MeshRenderer::Render()
{
	GetTransform()->Push();

	_material->Push();
	_mesh->Render();
}
