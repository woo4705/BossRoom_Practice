#include "PacketManager.h"

void PacketManager::Init(GameServer* pServer)
{
	m_RecvFuntionDictionary = std::unordered_map<int, PROCESS_RECV_PACKET_FUNCTION>();

	m_RecvFuntionDictionary[(int)PacketDefine::PACKET_ID::PACKET_ID_PACKET_LOGIN_REQ] = &PacketManager::ProcessLogin;
	m_RecvFuntionDictionary[(int)PacketDefine::PACKET_ID::PACKET_ID_PACKET_CREATE_LOBBY_REQ] = &PacketManager::ProcessCreateLobby;
	m_RecvFuntionDictionary[(int)PacketDefine::PACKET_ID::PACKET_ID_PACKET_ENTER_LOBBY_REQ] = &PacketManager::ProcessEnterLobby;
	m_RecvFuntionDictionary[(int)PacketDefine::PACKET_ID::PACKET_ID_PACKET_LEAVE_LOBBY_REQ] = &PacketManager::ProcessLeaveLobby;

	m_pServer = pServer;
}

void PacketManager::ProcessRecvPacket(Session::SessionRef session, uint8_t* pBuf, INT16 copySize)
{
	// 패킷헤더 조립
	auto root = PacketDefine::GetPacket(pBuf);
	PacketDefine::PACKET_ID pHeader = root ->packet_id_type();

	auto iter = m_RecvFuntionDictionary.find(pHeader);
	if (iter != m_RecvFuntionDictionary.end())
	{
		(this->*(iter->second))(session, pBuf, copySize);
	}
}

void PacketManager::ProcessLogin(Session::SessionRef session, uint8_t* pBuf, INT16 copySize)
{
	auto root = PacketDefine::GetPacket(pBuf);
	auto raw = static_cast<const PacketDefine::PACKET_LOGIN_REQ*>(root->packet_id());

}

void PacketManager::ProcessCreateLobby(Session::SessionRef session, uint8_t* pBuf, INT16 copySize)
{
	auto root = PacketDefine::GetPacket(pBuf);
	auto raw = static_cast<const PacketDefine::PACKET_LOGIN_REQ*>(root->packet_id());
}

void PacketManager::ProcessEnterLobby(Session::SessionRef session, uint8_t* pBuf, INT16 copySize)
{
}

void PacketManager::ProcessLeaveLobby(Session::SessionRef session, uint8_t* pBuf, INT16 copySize)
{
}
