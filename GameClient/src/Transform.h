#pragma once

#include "Component.h"

class Transform : public Component
{
public:
	Transform();
	virtual ~Transform();

public:
	virtual void FinalUpdate() override;

public:
	const glm::vec3&				GetLocalPosition() { return _localPosition; }
	const glm::vec3&				GetLocalRotation() { return _localRotation; }
	const glm::vec3&				GetLocalScale() { return _localScale; }

	const glm::mat4&				GetLocalToWorldMatrix() { return _worldMatrix; }
	const glm::vec3&				GetWorldPosition() { return glm::vec3(_worldMatrix[3]); }

	glm::vec3						GetRight() { return glm::vec3(_worldMatrix[0][0], _worldMatrix[1][0], _worldMatrix[2][0]); }
	glm::vec3						GetUp() { return glm::vec3(_worldMatrix[0][1], _worldMatrix[1][1], _worldMatrix[2][1]); }
	glm::vec3						GetLook() { return glm::vec3(-_worldMatrix[0][2], -_worldMatrix[1][2], -_worldMatrix[2][2]); }

	void							SetLocalPosition(const glm::vec3& position) { _localPosition = position; }
	void							SetLocalRotation(const glm::vec3& rotation) { _localRotation = rotation; }
	void							SetLocalScale(const glm::vec3& sacle) { _localScale = sacle; }

	void							SetParent(std::shared_ptr<Transform> parent) { _parent = parent; }
	std::weak_ptr<Transform>		GetParent() { return _parent; }

private:
	glm::vec3 _localPosition = {};
	glm::vec3 _localRotation = {};
	glm::vec3 _localScale = glm::vec3(1.f);

	glm::mat4 _localMatrix = {};
	glm::mat4 _worldMatrix = {};

	std::weak_ptr<Transform> _parent;
};