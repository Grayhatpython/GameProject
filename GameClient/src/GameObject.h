#pragma once
#include "Component.h"

class Transform;
class MeshRenderer;
class Camera;
class MonoBehaviour;

class GameObject : public std::enable_shared_from_this<GameObject>
{
public:
	GameObject();
	virtual ~GameObject();

public:
	void Awake();
	void Start();
	void Update();
	void LateUpdate();
	void FinalUpdate();

public:
	void AddComponent(std::shared_ptr<Component> component);

public:
	std::shared_ptr<Transform>		GetTransform();
	std::shared_ptr<MeshRenderer>	GetMeshRenderer();
	std::shared_ptr<Camera>			GetCamera();

private:
	//	TODO
	std::shared_ptr<Component>		GetComponent(ComponentType type);

private:
	std::array<std::shared_ptr<Component>, COMPONENT_COUNT> _components;
	std::vector<std::shared_ptr<MonoBehaviour>>				_scripts;
};














