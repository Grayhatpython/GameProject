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

	// ���Ϸ� ���� ���ʹϾ����� ��ȯ
	glm::quat quaternion = glm::quat(eulerAngles);

	// ���ʹϾ��� 4x4 ȸ�� ��ķ� ��ȯ
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
