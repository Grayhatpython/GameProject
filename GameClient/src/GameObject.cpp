#include "GameObject.h"
#include "Transform.h"
#include "MeshRenderer.h"
#include "MonoBehaviour.h"

GameObject::GameObject()
{

}

GameObject::~GameObject()
{

}

void GameObject::Awake()
{
	for (auto& component : _components)
	{
		if (component)
			component->Awake();
	}

	for (auto& script : _scripts)
		script->Awake();
}

void GameObject::Start()
{
	for (auto& component : _components)
	{
		if (component)
			component->Start();
	}

	for (auto& script : _scripts)
		script->Start();
}

void GameObject::Update()
{
	for (auto& component : _components)
	{
		if (component)
			component->Update();
	}

	for (auto& script : _scripts)
		script->Update();
}

void GameObject::LateUpdate()
{
	for (auto& component : _components)
	{
		if (component)
			component->LateUpdate();
	}

	for (auto& script : _scripts)
		script->LateUpdate();
}

void GameObject::FinalUpdate()
{
	for (auto& component : _components)
	{
		if (component)
			component->FinalUpdate();
	}
}

void GameObject::AddComponent(std::shared_ptr<Component> component)
{
	component->SetGameObject(shared_from_this());

	uint8_t index = static_cast<uint8_t>(component->GetType());
	if (index < COMPONENT_COUNT)
		_components[index] = component;
	else
		_scripts.push_back(std::static_pointer_cast<MonoBehaviour>(component));
}

std::shared_ptr<Transform> GameObject::GetTransform()
{
	auto component = GetComponent(ComponentType::Transform);
	return std::static_pointer_cast<Transform>(component);
}

std::shared_ptr<MeshRenderer> GameObject::GetMeshRenderer()
{
	auto component = GetComponent(ComponentType::MeshRenderer);
	return std::static_pointer_cast<MeshRenderer>(component);
}

std::shared_ptr<Camera> GameObject::GetCamera()
{
	auto component = GetComponent(ComponentType::Camera);
	return std::static_pointer_cast<Camera>(component);
}

std::shared_ptr<Component> GameObject::GetComponent(ComponentType type)
{
	uint8_t index = static_cast<uint8_t>(type);
	_ASSERT(index < COMPONENT_COUNT);
	return _components[index];
}
