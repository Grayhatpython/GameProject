#include "SceneManager.h"
#include "Scene.h"
#include "GameObject.h"
#include "Mesh.h"
#include "MeshRenderer.h"
#include "Image.h"
#include "Texture.h"
#include "Transform.h"
#include "Camera.h"

std::shared_ptr<SceneManager>	SceneManager::S_Instance = nullptr;
std::once_flag					SceneManager::S_InitializeFlag;

void SceneManager::Update()
{
	if (_currentScene == nullptr)
		return;

	_currentScene->Update();
	_currentScene->LateUpdate();
	_currentScene->FinalUpdate();
}

void SceneManager::Render()
{
}

void SceneManager::Clear()
{

}

//	TODO : TEST
void SceneManager::LoadScene(const std::wstring& sceneName)
{
	_currentScene = std::make_shared<Scene>();

	{
		std::shared_ptr<GameObject> container = std::make_shared<GameObject>();
		std::shared_ptr<GameObject> box1 = std::make_shared<GameObject>();
		std::shared_ptr<GameObject> box2 = std::make_shared<GameObject>();

		container->AddComponent(std::make_shared<Transform>());
		box1->AddComponent(std::make_shared<Transform>());
		box2->AddComponent(std::make_shared<Transform>());

		std::shared_ptr<MeshRenderer> containerMeshRenderer = std::make_shared<MeshRenderer>();
		std::shared_ptr<MeshRenderer> box1MeshRenderer = std::make_shared<MeshRenderer>();
		std::shared_ptr<MeshRenderer> box2MeshRenderer = std::make_shared<MeshRenderer>();

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

		std::shared_ptr<Mesh> mesh = std::make_shared<Mesh>();
		mesh->Initialize(vertices, indices, GL_TRIANGLES);

		containerMeshRenderer->SetMesh(mesh);
		box1MeshRenderer->SetMesh(mesh);
		box2MeshRenderer->SetMesh(mesh);

		std::unique_ptr<Image> darkGrayImage = std::make_unique<Image>(new Image());
		_ASSERT(darkGrayImage->CreateSingleColorImage(4, 4, glm::vec4(0.2f, 0.2f, 0.2f, 1.0f)));

		std::shared_ptr<Texture> darkGrayTexture = std::make_shared<Texture>();
		darkGrayTexture->Initialize(darkGrayImage.get());

		std::unique_ptr<Image> grayImage = std::make_unique<Image>(new Image());
		_ASSERT(grayImage->CreateSingleColorImage(4, 4, glm::vec4(0.2f, 0.2f, 0.2f, 1.0f)));

		std::shared_ptr<Texture> grayTexture = std::make_shared<Texture>();
		grayTexture->Initialize(grayImage.get());

		std::shared_ptr<Material>planeMaterial = std::make_shared<Material>();

		std::unique_ptr<Image> marbleImage = std::make_unique<Image>(new Image());
		_ASSERT(marbleImage->LoadFromFile("../image/marble.jpg"));

		std::shared_ptr<Texture> marbleTexture = std::make_shared<Texture>();
		marbleTexture->Initialize(marbleImage.get());

		planeMaterial->_diffuse = marbleTexture;
		planeMaterial->_specular = grayTexture;
		planeMaterial->_shininess = 128.0f;

		std::shared_ptr<Material>box1Material = std::make_shared<Material>();

		std::unique_ptr<Image> containerImage = std::make_unique<Image>(new Image());
		_ASSERT(containerImage->LoadFromFile("../image/container.jpg"));

		std::shared_ptr<Texture> containerTexture = std::make_shared<Texture>();
		containerTexture->Initialize(containerImage.get());

		box1Material->_specular = darkGrayTexture;
		box1Material->_shininess = 16.0f;

		std::shared_ptr<Material>box2Material = std::make_shared<Material>();

		std::unique_ptr<Image> container2Image = std::make_unique<Image>(new Image());
		_ASSERT(container2Image->LoadFromFile("../image/container2.jpg"));

		std::shared_ptr<Texture> container2Texture = std::make_shared<Texture>();
		container2Texture->Initialize(container2Image.get());

		std::unique_ptr<Image> container2_specularImage = std::make_unique<Image>(new Image());
		_ASSERT(container2_specularImage->LoadFromFile("../image/container2_specular.jpg"));

		std::shared_ptr<Texture> container2_specularTexture = std::make_shared<Texture>();
		container2_specularTexture->Initialize(container2_specularImage.get());

		box2Material->_shininess = 64.0f;

		containerMeshRenderer->SetMaterial(planeMaterial);
		box1MeshRenderer->SetMaterial(box1Material);
		box2MeshRenderer->SetMaterial(box2Material);

		_currentScene->AddGameObject(container);
		_currentScene->AddGameObject(box1);
		_currentScene->AddGameObject(box2);

		auto camera = std::make_shared<GameObject>();
		camera->AddComponent(std::make_shared<Transform>());
		camera->AddComponent(std::make_shared<Camera>());
		//camera->AddComponent(std::make_shared<MeshRenderer>());
	}

	_currentScene->Awake();
	_currentScene->Start();
}
