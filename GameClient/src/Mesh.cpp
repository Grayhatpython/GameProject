#include "Mesh.h"
#include "Buffer.h"
#include "Program.h"
#include "Texture.h"

//	Meterial
void Material::SetProgram(const Program* program) const
{
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


//	Mesh
std::unique_ptr<Mesh> Mesh::Create(const std::vector<Vertex>& vertices, const std::vector<uint32_t>& indices, uint32_t primitiveType)
{
	auto mesh = std::unique_ptr<Mesh>(new Mesh());
	mesh->Initialize(vertices, indices, primitiveType);
	return std::move(mesh);
}

std::unique_ptr<Mesh> Mesh::CreateBox()
{

	/*
	//	x y z	: position
	//	r g b	: color
	//	s t		: texture coordinate
	float vertices[] = {
	  0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f,	// top right
	  0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,	// bottom right
	  -0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, // bottom left
	  -0.5f, 0.5f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f,	// top left
	};

	uint32_t indices[] = {
		0, 1, 3,
		1, 2, 3
	};


	// 정점이 3개, 각 정점의 위치, 위치에 대해 x/y/z 값, 각 좌표값마다 float(4byte) 크기, 정점 간의 간격이 12bytes
	// vertexBuffer object가 가진 정점에 대한 구조를 알려줘야 한다!

	// Vertex Arrray Object (VAO)
	// 정점 데이터의 구조를 알려주는 object

	_vertexLayout = VertexLayout::Create();

	_vertexBuffer = Buffer::Create(GL_ARRAY_BUFFER, GL_STATIC_DRAW, vertices, sizeof(vertices));
	assert(_vertexBuffer);

	// vertex buffer layout
	// vertex shader -> layout을 사용하여 attrib index 지정
	// vertex shader out 변수들은 Rasterization 과정을 거쳐 픽셀단위로 보간되어 fragment shader의 in 변수들로 입력
	// vertex -> x | y | z | r | g | b | s | t
	// 0 attrib ( xyz, offset 0 )
	_vertexLayout->EnableVertexAttribArray(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 8, 0);
	// 1 attrib ( rgb, offset 12 )
	_vertexLayout->EnableVertexAttribArray(1, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 8, sizeof(float) * 3);
	// 2 attrib ( st, offset 24 )
	_vertexLayout->EnableVertexAttribArray(2, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 8, sizeof(float) * 6);

	_indexBuffer = Buffer::Create(GL_ELEMENT_ARRAY_BUFFER, GL_STATIC_DRAW, indices, sizeof(indices));
	assert(_indexBuffer);

	*/

	// pos.xyz, normal.xyz, texcoord.uv

    std::vector<Vertex> vertices = {
	  Vertex { glm::vec3(-0.5f, -0.5f, -0.5f), glm::vec3(0.0f,  0.0f, -1.0f), glm::vec2(0.0f, 0.0f) },
	  Vertex { glm::vec3(0.5f, -0.5f, -0.5f), glm::vec3(0.0f,  0.0f, -1.0f), glm::vec2(1.0f, 0.0f) },
	  Vertex { glm::vec3(0.5f,  0.5f, -0.5f), glm::vec3(0.0f,  0.0f, -1.0f), glm::vec2(1.0f, 1.0f) },
	  Vertex { glm::vec3(-0.5f,  0.5f, -0.5f), glm::vec3(0.0f,  0.0f, -1.0f), glm::vec2(0.0f, 1.0f) },

	  Vertex { glm::vec3(-0.5f, -0.5f,  0.5f), glm::vec3(0.0f,  0.0f,  1.0f), glm::vec2(0.0f, 0.0f) },
	  Vertex { glm::vec3(0.5f, -0.5f,  0.5f), glm::vec3(0.0f,  0.0f,  1.0f), glm::vec2(1.0f, 0.0f) },
	  Vertex { glm::vec3(0.5f,  0.5f,  0.5f), glm::vec3(0.0f,  0.0f,  1.0f), glm::vec2(1.0f, 1.0f) },
	  Vertex { glm::vec3(-0.5f,  0.5f,  0.5f), glm::vec3(0.0f,  0.0f,  1.0f), glm::vec2(0.0f, 1.0f) },

	  Vertex { glm::vec3(-0.5f,  0.5f,  0.5f), glm::vec3(-1.0f,  0.0f,  0.0f), glm::vec2(1.0f, 0.0f) },
	  Vertex { glm::vec3(-0.5f,  0.5f, -0.5f), glm::vec3(-1.0f,  0.0f,  0.0f), glm::vec2(1.0f, 1.0f) },
	  Vertex { glm::vec3(-0.5f, -0.5f, -0.5f), glm::vec3(-1.0f,  0.0f,  0.0f), glm::vec2(0.0f, 1.0f) },
	  Vertex { glm::vec3(-0.5f, -0.5f,  0.5f), glm::vec3(-1.0f,  0.0f,  0.0f), glm::vec2(0.0f, 0.0f) },

	  Vertex { glm::vec3(0.5f,  0.5f,  0.5f), glm::vec3(1.0f,  0.0f,  0.0f), glm::vec2(1.0f, 0.0f) },
	  Vertex { glm::vec3(0.5f,  0.5f, -0.5f), glm::vec3(1.0f,  0.0f,  0.0f), glm::vec2(1.0f, 1.0f) },
	  Vertex { glm::vec3(0.5f, -0.5f, -0.5f), glm::vec3(1.0f,  0.0f,  0.0f), glm::vec2(0.0f, 1.0f) },
	  Vertex { glm::vec3(0.5f, -0.5f,  0.5f), glm::vec3(1.0f,  0.0f,  0.0f), glm::vec2(0.0f, 0.0f) },

	  Vertex { glm::vec3(-0.5f, -0.5f, -0.5f), glm::vec3(0.0f, -1.0f,  0.0f), glm::vec2(0.0f, 1.0f) },
	  Vertex { glm::vec3(0.5f, -0.5f, -0.5f), glm::vec3(0.0f, -1.0f,  0.0f), glm::vec2(1.0f, 1.0f) },
	  Vertex { glm::vec3(0.5f, -0.5f,  0.5f), glm::vec3(0.0f, -1.0f,  0.0f), glm::vec2(1.0f, 0.0f) },
	  Vertex { glm::vec3(-0.5f, -0.5f,  0.5f), glm::vec3(0.0f, -1.0f,  0.0f), glm::vec2(0.0f, 0.0f) },

	  Vertex { glm::vec3(-0.5f,  0.5f, -0.5f), glm::vec3(0.0f,  1.0f,  0.0f), glm::vec2(0.0f, 1.0f) },
	  Vertex { glm::vec3(0.5f,  0.5f, -0.5f), glm::vec3(0.0f,  1.0f,  0.0f), glm::vec2(1.0f, 1.0f) },
	  Vertex { glm::vec3(0.5f,  0.5f,  0.5f), glm::vec3(0.0f,  1.0f,  0.0f), glm::vec2(1.0f, 0.0f) },
	  Vertex { glm::vec3(-0.5f,  0.5f,  0.5f), glm::vec3(0.0f,  1.0f,  0.0f), glm::vec2(0.0f, 0.0f) },
    };

    std::vector<uint32_t> indices = {
       0,  2,  1,  2,  0,  3,
       4,  5,  6,  6,  7,  4,
       8,  9, 10, 10, 11,  8,
      12, 14, 13, 14, 12, 15,
      16, 17, 18, 18, 19, 16,
      20, 22, 21, 22, 20, 23,
    };

    return Create(vertices, indices, GL_TRIANGLES);
}

void Mesh::Render(const Program* program) const
{
	_vertexLayout->Bind();

	if (_material) 
		_material->SetProgram(program);

	glDrawElements(_primitiveType, static_cast<GLsizei>(_indexBuffer->GetCount()), GL_UNSIGNED_INT, 0);
}

void Mesh::Initialize(const std::vector<Vertex>& vertices, const std::vector<uint32_t>& indices, uint32_t primitiveType)
{
	_vertexLayout = VertexLayout::Create();
	_vertexBuffer = Buffer::Create(GL_ARRAY_BUFFER, GL_STATIC_DRAW, vertices.data(), sizeof(Vertex), vertices.size());
	_indexBuffer = Buffer::Create(GL_ELEMENT_ARRAY_BUFFER, GL_STATIC_DRAW, indices.data(), sizeof(uint32_t), indices.size());

	//	0 attrib ( pos.xyz, offset 0 )
	_vertexLayout->EnableVertexAttribArray(0, 3, GL_FLOAT, false, sizeof(Vertex), 0);
	//	1 attrib ( normal.xyz, offset 12 )
	_vertexLayout->EnableVertexAttribArray(1, 3, GL_FLOAT, false, sizeof(Vertex), offsetof(Vertex, normal));
	//	2 attrib ( texcoord.uv, offset 24 )
	_vertexLayout->EnableVertexAttribArray(2, 2, GL_FLOAT, false, sizeof(Vertex), offsetof(Vertex, textureCoord));
}

