#include "Mesh.h"
#include "Buffer.h"
#include "Program.h"
#include "Texture.h"

//	Meterial
void Material::Push(const Program* program) const
{
	//	texture�� shader program�� �����ϴ� ���
	//	glActiveTexture(textureSlot)			->	���� �ٷ���� �ϴ� texture slot ����
	//	glBindTexture(textureType, textureID)	->	���� �������� texture slot�� texture object binding
	//	glGetUniformLocation()					->	shader ���� sampler2D uniform handle�� ���´�.
	//	glUniform1i()							->	sampler2D uniform�� texture slot index �Է�

	int textureCount = 0;
	if (_diffuse) 
	{
		glActiveTexture(GL_TEXTURE0 + textureCount);
		program->SetUniform("material.diffuse", textureCount);
		_diffuse->Bind();
		textureCount++;
	}
	if (_specular) 
	{
		glActiveTexture(GL_TEXTURE0 + textureCount);
		program->SetUniform("material.specular", textureCount);
		_specular->Bind();
		textureCount++;
	}
	glActiveTexture(GL_TEXTURE0);
	program->SetUniform("material.shininess", _shininess);
}

void Mesh::Render() const
{
	_vertexLayout->Bind();

	glDrawElements(_primitiveType, static_cast<GLsizei>(_indexBuffer->GetCount()), GL_UNSIGNED_INT, 0);
}

void Mesh::Initialize(const std::vector<Vertex>& vertices, const std::vector<uint32_t>& indices, uint32_t primitiveType)
{
	_vertexLayout = std::make_unique<VertexLayout>(new VertexLayout());
	_vertexLayout->Initialize();

	_vertexBuffer = std::make_shared<Buffer>();
	_ASSERT(_vertexBuffer->Initialize(GL_ARRAY_BUFFER, GL_STATIC_DRAW, vertices.data(), sizeof(Vertex), vertices.size()));
	
	_indexBuffer = std::make_shared<Buffer>();
	_ASSERT(_indexBuffer->Initialize(GL_ELEMENT_ARRAY_BUFFER, GL_STATIC_DRAW, indices.data(), sizeof(uint32_t), indices.size()));

	//	0 attrib ( pos.xyz, offset 0 )
	_vertexLayout->EnableVertexAttribArray(0, 3, GL_FLOAT, false, sizeof(Vertex), 0);
	//	1 attrib ( normal.xyz, offset 12 )
	_vertexLayout->EnableVertexAttribArray(1, 3, GL_FLOAT, false, sizeof(Vertex), offsetof(Vertex, normal));
	//	2 attrib ( texcoord.uv, offset 24 )
	_vertexLayout->EnableVertexAttribArray(2, 2, GL_FLOAT, false, sizeof(Vertex), offsetof(Vertex, textureCoord));
}

