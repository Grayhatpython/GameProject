#pragma once

enum class ComponentType : uint8_t
{
	Transform,
	MeshRenderer,
	MonoBehaviour,
	End,
};

enum
{
	COMPONENT_COUNT = static_cast<uint8_t>(ComponentType::End) - 1,
};

class GameObject;
class Transform;

class Component
{
public:
	Component(ComponentType type);
	virtual ~Component();

public:
	virtual void Awake() {}
	virtual void Start() {}
	virtual void Update() {}
	virtual void LateUpdate() {}
	virtual void FinalUpdate() {}

private:
	friend class GameObject;
	void SetGameObject(std::shared_ptr<GameObject> gameObject) { _gameObject = gameObject; }

public:
	bool						IsExpired() { return _gameObject.expired() == false; }

	ComponentType				GetType() { return _type; }

	std::shared_ptr<GameObject> GetGameObject();
	std::shared_ptr<Transform>	GetTransform();

private:
	ComponentType				_type;
	std::weak_ptr<GameObject>	_gameObject;
};