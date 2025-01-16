#pragma once

using GameObjectCreateFunc = std::function<GameObjectRef()>;

class GameObjectManager
{
public:	
	void Initialize();

	template<typename T>
	void RegisterGameObjectCreateFunc()
	{
		ASSERT_CRASH(_gameObjectCreateFuncMap.find(T::ClassId) == _gameObjectCreateFuncMap.end());
		_gameObjectCreateFuncMap[T::ClassId] = T::CreateInstance;
	}

	GameObjectRef Create(uint32 classId);

private:
	std::unordered_map<uint32, GameObjectCreateFunc> _gameObjectCreateFuncMap;

	Atomic<uint32> autoincrementGameObjectId = 1;
};

extern GameObjectManager* GGameObjectManager;

