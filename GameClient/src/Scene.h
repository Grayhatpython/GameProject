#pragma once

class GameObject;
class Scene
{
public:
	void Awake();
	void Start();
	void Update();
	void LateUpdate();
	void FinalUpdate();

public:
	void AddGameObject(std::shared_ptr<GameObject> gameObject);
	void RemoveGameObject(std::shared_ptr<GameObject> gameObject);

public:
	const std::vector<std::shared_ptr<GameObject>>& GetGameObjects() { return _gameObjects; }

private:
	std::vector<std::shared_ptr<GameObject>> _gameObjects;
};