#include "MeshRenderer.h"
#include "Mesh.h"
#include "Transform.h"
#include "Program.h"

MeshRenderer::MeshRenderer()
	: Component(ComponentType::MeshRenderer)
{

}

MeshRenderer::~MeshRenderer()
{

}

void MeshRenderer::Render(const std::shared_ptr<Program>& program)
{
	GetTransform()->PushData();

	_material->PushData(program);
	_mesh->Render();
}
