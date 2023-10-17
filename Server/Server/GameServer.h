#pragma once
#include "pch.h"

//using boost::asio::ip::tcp;

class GameServer
{
public:
	GameServer(uint16_t port);
	bool Start();
	void Stop();
	void Update(size_t nMaxMessages, bool bWait);

	void WaitForClientConnection();

	void MessageClient(Session::SessionRef client, const char* msg);

	void MessageAllClient(const char* msg, Session::SessionRef pIgnoreClient)
	{

	}

private:
	bool OnClientConnect(Session::SessionRef newSession) {
		return false;
	}

	void OnClientDisonnect(Session::SessionRef newSession);
	void ProcessPacket(Session::SessionRef newSession, PacketMessage msg);

	ThreadSafeQueue<std::pair<Session::SessionRef, PacketMessage>>m_qPacketIn; 
	std::deque<std::shared_ptr<Session>> m_deqSession;
	
	boost::asio::io_context m_ioContext;
	std::thread m_threadContext;
	boost::asio::ip::tcp::acceptor m_acceptor;
	//boost::asio::io_service::strand m_strand;
	std::unique_ptr<PacketManager> m_pPacketManager;

	uint32_t m_nIdCounter = 10000;
};

