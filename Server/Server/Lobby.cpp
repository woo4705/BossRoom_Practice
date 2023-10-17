#pragma once
#include "Lobby.h"

void Lobby::Enter(std::shared_ptr<Session> session)
{
	m_SessionSet.insert(session);
	std::cout << session->GetSessionId() << " EnteredRoom" << endl;
}

void Lobby::Leave(std::shared_ptr<Session> session)
{
	m_SessionSet.erase(session);
}
/*
void Lobby::Broadcast(const PacketMessage& msg)
{
	for (auto session : m_SessionSet) {
		// send
	}
}
*/