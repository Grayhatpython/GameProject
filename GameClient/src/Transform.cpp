#include "Transform.h"
#include "Camera.h"

Transform::Transform()
	: Component(ComponentType::Transform)
{

}

Transform::~Transform()
{

}

void Transform::FinalUpdate()
{
	/*
	#include <glm/gtc/quaternion.hpp>
	#include <glm/gtx/quaternion.hpp>

	glm::vec3 eulerAngles = glm::vec3(glm::radians(30.0f), glm::radians(45.0f), glm::radians(60.0f));

	// 오일러 각을 쿼터니언으로 변환
	glm::quat quaternion = glm::quat(eulerAngles);

	// 쿼터니언을 4x4 회전 행렬로 변환
	glm::mat4 rotationMatrix = glm::toMat4(quaternion);
	*/

	//	TODO
	_localMatrix =
		glm::translate(glm::mat4(1.0f), _localPosition) *
		glm::rotate(glm::mat4(1.0f), glm::radians(_localRotation.x), glm::vec3(1.0f, 0.0f, 0.0f)) *
		glm::rotate(glm::mat4(1.0f), glm::radians(_localRotation.y), glm::vec3(0.0f, 1.0f, 0.0f)) *
		glm::rotate(glm::mat4(1.0f), glm::radians(_localRotation.z), glm::vec3(0.0f, 0.0f, 1.0f)) *
		glm::scale(glm::mat4(1.0f), _localScale);

	_worldMatrix = _localMatrix;

	std::shared_ptr<Transform> parent = GetParent().lock();
	if (parent != nullptr)
		_worldMatrix *= parent->GetLocalToWorldMatrix();
}

void Transform::PushData()
{
	//	TODO
	//_worldMatrix* Camera::S_ViewMatrix* Camera::S_ProjectionMatrix;
}
