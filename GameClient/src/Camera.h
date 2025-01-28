#pragma once
#include "Component.h"

enum class ProjectionType
{
	Perspective,	//	원근
	Orthographic,	//	직교
};

class Camera : public Component
{
public:
	Camera();
	virtual ~Camera();

public:
	virtual void	FinalUpdate() override;

	void			Render();

private:
	ProjectionType _type = ProjectionType::Perspective;

	float _near = 1.f;
	float _far = 50.f;
	float _fov = glm::radians(45.0f);

	glm::mat4 _viewMatrix = {};
	glm::mat4 _projectionMatrix = {};

public:
	static glm::mat4 S_ViewMatrix;
	static glm::mat4 S_ProjectionMatrix;
};