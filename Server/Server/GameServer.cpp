#include "GameServer.h"
//ThreadSafeQueue<int> m_qPacketIn(100);

GameServer::GameServer(uint16_t port)
	: m_acceptor(m_ioContext, boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), port)),
	m_qPacketIn(10)
{

}

bool GameServer::Start()
{
	m_pPacketManager = std::make_unique<PacketManager>();

	try {
		WaitForClientConnection();
		m_threadContext = std::thread([this]() {m_ioContext.run(); });

	}
	catch (std::exception& e) {
		std::cerr << "[Server] Exception :" << e.what() << std::endl;
		return false;
	}
	std::cout << "¡¼ SERVER START ¡½" << std::endl;
	return true;
}

void GameServer::Stop()
{
	m_ioContext.stop();
	if (m_threadContext.joinable()) {
		m_threadContext.join();
	}
}

void GameServer::Update(size_t nMaxMessages = -1, bool bWait = false)
{
	// Process as many messages as you can up to the value
	// specified
	size_t nMessageCount = 0;
	while (nMessageCount < nMaxMessages && m_qPacketIn.Size() != 0 )
	{
		// Grab the front message
		auto msg = m_qPacketIn.Front();

		// Pass to message handler
		ProcessPacket(msg.first, msg.second);

		nMessageCount++;
	}
}

void GameServer::WaitForClientConnection()
{
	m_acceptor.async_accept(
		[this] (std::error_code errCode, boost::asio::ip::tcp::socket socket) {
		if (!errCode) {
			std::cout << "[SERVER]New Connection" << socket.remote_endpoint() << std::endl;
			Session::SessionRef newSession =
				Session::create(m_ioContext, std::move(socket));

			if (OnClientConnect(newSession)) {
				m_deqSession.push_back(std::move(newSession));

				m_deqSession.back()->Start(m_nIdCounter++);
				std::cout << "[" << m_nIdCounter - 1 << "] Connection Approved\n";
			}

		}
		else {
			std::cout << "[SERVER] Connection Error" << errCode.message() << std::endl;
		}

		WaitForClientConnection();
	}
	);
}



void GameServer::MessageClient(Session::SessionRef session, const char* msg)
{
	if (session && session->isConnected()) {
		session->Send(msg);
	}
	else {
		OnClientDisonnect(session);
		session.reset();
		m_deqSession.erase(
			std::remove(m_deqSession.begin(), m_deqSession.end(), session), m_deqSession.end()
		);
	}
}

void GameServer::OnClientDisonnect(Session::SessionRef newSession)
{
}

void GameServer::ProcessPacket(Session::SessionRef session, PacketMessage packet)
{

	auto verifier = flatbuffers::Verifier(packet.body(), packet.body_length());
	if (PacketDefine::VerifyPacketBuffer(verifier) == false) {
		return;
	}

	m_pPacketManager->ProcessRecvPacket(session, packet.body(), packet.body_length());
}

