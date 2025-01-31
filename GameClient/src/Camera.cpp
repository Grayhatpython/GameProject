#include "Camera.h"
#include "Transform.h"
#include "Scene.h"
#include "SceneManager.h"
#include "GameObject.h"
#include "MeshRenderer.h"
#include "Program.h"

glm::mat4 Camera::S_ViewMatrix;
glm::mat4 Camera::S_ProjectionMatrix;

Camera::Camera()
	: Component(ComponentType::Camera)
{

}

Camera::~Camera()
{

}

void Camera::FinalUpdate()
{
	//_cameraFront = glm::rotate(glm::mat4(1.0f), glm::radians(_cameraYaw), glm::vec3(0.0f, 1.0f, 0.0f)) *
	//	glm::rotate(glm::mat4(1.0f), glm::radians(_cameraPitch), glm::vec3(1.0f, 0.0f, 0.0f)) *
	//	glm::vec4(0.0f, 0.0f, -1.0f, 0.0f);

	auto transform = GetTransform();

	//	TODO 
	//	width , height 고정값 변경
	_projectionMatrix = glm::perspective(_fov, static_cast<float>(WINDOW_WIDTH / WINDOW_HEIGHT), _near, _far);
	_viewMatrix = glm::inverse(transform->GetLocalToWorldMatrix());

	//auto view = glm::lookAt(_cameraPos, _cameraPos + _cameraFront, _cameraUp);

	S_ViewMatrix = _viewMatrix;
	S_ProjectionMatrix = _projectionMatrix;
}

void Camera::Render(const std::shared_ptr<Program>& program)
{
	auto scene = SceneManager::GetInstance()->GetCurrentScene();

	const auto& gameObjects = scene->GetGameObjects();

	for (const auto& gameObject : gameObjects)
	{
		auto meshRenderer = gameObject->GetMeshRenderer();
		if (meshRenderer == nullptr)
			continue;

		meshRenderer->Render(program);
	}
}
