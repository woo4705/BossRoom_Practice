#pragma once
#include "pch.h"

class PacketManager
{

public:
	PacketManager() {};
	~PacketManager() {};

	void Init(GameServer* pServer);

	void ProcessRecvPacket  (Session::SessionRef session, uint8_t* pBuf, INT16 copySize);
	void ProcessLogin         (Session::SessionRef session, uint8_t* pBuf, INT16 copySize);
	void ProcessCreateLobby(Session::SessionRef session, uint8_t* pBuf, INT16 copySize);
	void ProcessEnterLobby (Session::SessionRef session,  uint8_t* pBuf, INT16 copySize);
	void ProcessLeaveLobby (Session::SessionRef session, uint8_t* pBuf, INT16 copySize);


private:
	typedef void(PacketManager::* PROCESS_RECV_PACKET_FUNCTION)(Session::SessionRef, uint8_t*, INT16);
	std::unordered_map<int, PROCESS_RECV_PACKET_FUNCTION> m_RecvFuntionDictionary;

	GameServer* m_pServer;
};

