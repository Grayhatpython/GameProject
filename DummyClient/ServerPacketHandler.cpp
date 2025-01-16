#include "pch.h"
#include "ServerPacketHandler.h"
#include "ServerSession.h"
#include "Player.h"

PacketProcessingFunction GPacketPacketProcessingFunction[UINT16_MAX];

bool Packet_Processing_Function_Undefined(std::shared_ptr<PacketSession>& session, BYTE* buffer, int32 len)
{
	return true;
}

bool S_LOGIN_Packet_Processing_Function(std::shared_ptr<PacketSession>& session, Protocol::S_LOGIN& packet)
{
	auto serverSession = static_pointer_cast<ServerSession>(session);

	Protocol::C_ENTER_GAME enterGamePacket;
	enterGamePacket.set_playerindex(0);
	auto sendBuffer = ServerPacketHandler::MakeSendBuffer(enterGamePacket);
	serverSession->Send(sendBuffer);

	return true;
}

bool S_ENTER_GAME_Packet_Processing_Function(std::shared_ptr<PacketSession>& session, Protocol::S_ENTER_GAME& packet)
{
	//	플레이어 생성
	auto serverSession = static_pointer_cast<ServerSession>(session);
	const auto& player = packet.player();

	serverSession->_player = MakeShared<Player>();
	serverSession->_player->_id = player.objectid();
	serverSession->_player->_positionInfo->CopyFrom(player.positioninfo());

	return true;
}

bool S_LEAVE_GAME_Packet_Processing_Function(std::shared_ptr<PacketSession>& session, Protocol::S_LEAVE_GAME& packet)
{
	auto serverSession = static_pointer_cast<ServerSession>(session);

	return true;
}

bool S_SPAWN_Packet_Processing_Function(std::shared_ptr<PacketSession>& session, Protocol::S_SPAWN& packet)
{
	auto serverSession = static_pointer_cast<ServerSession>(session);

	return true;
}

bool S_DESPAWN_Packet_Processing_Function(std::shared_ptr<PacketSession>& session, Protocol::S_DESPAWN& packet)
{
	return true;
}

bool S_MOVE_Packet_Processing_Function(std::shared_ptr<PacketSession>& session, Protocol::S_MOVE& packet)
{
	return true;
}

bool S_CHAT_Packet_Processing_Function(std::shared_ptr<PacketSession>& session, Protocol::S_CHAT& packet)
{
	return true;
}