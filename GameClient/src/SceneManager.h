#pragma once

class Scene;
class SceneManager
{
private:
	SceneManager() {};
	~SceneManager() {};

public:
	static std::shared_ptr<SceneManager> GetInstance()
	{
		std::call_once(S_InitializeFlag, []() {
				S_Instance = std::make_shared<SceneManager>();
			});

		return S_Instance;
	}

	SceneManager(const SceneManager&) = delete;
	SceneManager& operator=(const SceneManager&) = delete;

public:
	void Update();
	void Render();
	void Clear();
	//	TODO
	void LoadScene(const std::wstring& sceneName);

public:
	std::shared_ptr<Scene> GetCurrentScene() { return _currentScene; }

private:
	static std::shared_ptr<SceneManager>	S_Instance;
	static std::once_flag					S_InitializeFlag;

private:
	std::shared_ptr<Scene>		_currentScene;
};













