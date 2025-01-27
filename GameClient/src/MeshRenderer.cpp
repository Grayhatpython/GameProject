#include "MeshRenderer.h"
#include "Mesh.h"

MeshRenderer::MeshRenderer()
	: Component(ComponentType::MeshRenderer)
{

}

MeshRenderer::~MeshRenderer()
{

}

void MeshRenderer::Render()
{
	//GetTransform()->Update();

	_material->Update();
	_mesh->Render();
}
