#pragma once

#define WIN32_LEAN_AND_MEAN // 거의 사용되지 않는 내용을 Windows 헤더에서 제외합니다.

#ifdef _DEBUG
#pragma comment(lib, "ServerCore\\Debug\\ServerCore.lib")
#pragma comment(lib, "Protobuf\\Debug\\libprotobufd.lib")
#else
#pragma comment(lib, "ServerCore\\Release\\ServerCore.lib")
#pragma comment(lib, "Protobuf\\Release\\libprotobuf.lib")
#endif

#include "CorePch.h"
#include "Enum.pb.h"
#include "Struct.pb.h"

using ClientSessionRef = std::shared_ptr<class ClientSession>;
using GameObjectRef = std::shared_ptr<class GameObject>;
using CreatureRef = std::shared_ptr<class Creature>;
using MonsterRef = std::shared_ptr<class Monster>;
using PlayerRef = std::shared_ptr<class Player>;
using RoomRef = std::shared_ptr<class Room>;