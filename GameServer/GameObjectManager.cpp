#include "pch.h"
#include "GameObjectManager.h"
#include "ClientSession.h"
#include "Room.h"
#include "Player.h"
#include "Monster.h"

GameObjectManager* GGameObjectManager = nullptr;

void GameObjectManager::Initialize()
{
	RegisterGameObjectCreateFunc<GameObject>();
	RegisterGameObjectCreateFunc<Monster>();
	RegisterGameObjectCreateFunc<Player>();
}

GameObjectRef GameObjectManager::Create(uint32 classId)
{
	const auto id = autoincrementGameObjectId.fetch_add(1);

	auto createFunc = _gameObjectCreateFuncMap[classId];
	auto gameObject = createFunc();

	gameObject->_objectInfo->set_objectid(id);
	gameObject->_positionInfo->set_objectid(id);

	return gameObject;
}