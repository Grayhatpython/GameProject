#include "pch.h"
#include "ClientPacketHandler.h"
#include "ClientSession.h"
#include "Room.h"
#include "Player.h"
#include "GameObjectManager.h"

PacketProcessingFunction GPacketPacketProcessingFunction[UINT16_MAX];

bool Packet_Processing_Function_Undefined(std::shared_ptr<PacketSession>& session, BYTE* buffer, int32 len)
{
	return true;
}

bool C_LOGIN_Packet_Processing_Function(std::shared_ptr<PacketSession>& session, Protocol::C_LOGIN& packet)
{
	//	Client -> Login Server -> Game Server
	//	인증을 확인하기 위해 Redis? 
	//	DB에서 Account 정보

	Protocol::S_LOGIN loginPacket;

	for (int32 i = 0; i < 2; i++)
	{
		auto player = loginPacket.add_players();
		auto positionInfo = player->mutable_positioninfo();

		positionInfo->set_x(Utils::GetRandom(0.f, 100.f));
		positionInfo->set_y(0);
		positionInfo->set_z(Utils::GetRandom(0.f, 100.f));
		positionInfo->set_yaw(Utils::GetRandom(0.f, 45.f));
	}

	loginPacket.set_successed(true);

	auto sendBuffer = ClientPacketHandler::MakeSendBuffer(loginPacket);
	session->Send(sendBuffer);

	return true;
}

bool C_ENTER_GAME_Packet_Processing_Function(std::shared_ptr<PacketSession>& session, Protocol::C_ENTER_GAME& packet)
{
	//	플레이어 생성
	auto clientSession = static_pointer_cast<ClientSession>(session);
	//	TEMP
	auto gameObject = GGameObjectManager->Create(Player::ClassId);
	auto player = std::static_pointer_cast<Player>(gameObject);

	player->_ownerSession = clientSession;
	clientSession->_player = player;

	if (player == nullptr)
		return false;

	//	Room Enter
	//	TEMP	
	GRoom->PushJob(&Room::Enter, true, static_pointer_cast<GameObject>(player));

	return true;
}

bool C_LEAVE_GAME_Packet_Processing_Function(std::shared_ptr<PacketSession>& session, Protocol::C_LEAVE_GAME& packet)
{
	auto clientSession = static_pointer_cast<ClientSession>(session);
	auto player = clientSession->_player;

	if (player == nullptr)
		return false;

	auto room = player->_room.lock();
	if (room == nullptr)
		return false;

	room->PushJob(&Room::Leave, true, player->_objectInfo->objectid());

	return true;
}

bool C_MOVE_Packet_Processing_Function(std::shared_ptr<PacketSession>& session, Protocol::C_MOVE& packet)
{
	TRACE_THREAD_CALL_STACK;

	auto clientSession = static_pointer_cast<ClientSession>(session);
	auto player = clientSession->_player;

	if (player == nullptr)
		return false;

	auto room = player->_room.lock();
	if (room == nullptr)
		return false;

	room->PushJob(&Room::Move, true, packet);

	return true;
}

bool C_CHAT_Packet_Processing_Function(std::shared_ptr<PacketSession>& session, Protocol::C_CHAT& packet)
{
	return true;
}

