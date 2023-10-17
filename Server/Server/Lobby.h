#pragma once
#include "pch.h"
class LobbyPlayer {
public:
	virtual ~LobbyPlayer() {};
	virtual void deliver(const std::string& msg) = 0;
};

typedef std::shared_ptr<Session> sessionRef;

class Lobby
{
	void Enter(std::shared_ptr<Session> session);
	void Leave(std::shared_ptr<Session> session);
	//void Broadcast(const PacketMessage& msg);

private:
	boost::asio::io_service::strand* m_strand;
	std::set<sessionRef> m_SessionSet;
};