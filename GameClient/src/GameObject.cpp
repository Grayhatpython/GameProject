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

void GameObject::Initialize()
{
	AddComponent(std::make_shared<Transform>());
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
	uint8_t index = static_cast<uint8_t>(ComponentType::Transform);
	return std::static_pointer_cast<Transform>(_components[index]);
}
