#pragma once
#include "pch.h"

class Session: public std::enable_shared_from_this<Session>
{
public:
	using SessionRef = std::shared_ptr<class Session>;
	
	static SessionRef create(boost::asio::io_context& ioContext, boost::asio::ip::tcp::socket socket) {
		return SessionRef(new Session(ioContext, std::move(socket)));
	}

	inline bool isConnected() const { return m_socket.is_open(); }
	inline uint32_t GetSessionId() const { return m_sessionId; }
	bool Send(const char* msg);

	boost::asio::ip::tcp::socket& socket() { return m_socket; }
	bool Start(uint32_t id);

private:
	Session(boost::asio::io_context& io_context, boost::asio::ip::tcp::socket socket);

	void readPacket(const boost::system::error_code& error);

	void handleRead(const boost::system::error_code& error, size_t bytes_transferred);
	void handleWrite(const boost::system::error_code& error, size_t bytes_transferred);

private:
	boost::asio::ip::tcp::socket m_socket;
	boost::asio::io_context& m_pIoContext;
	std::string m_msg;

	boost::asio::streambuf m_send_buf;
	boost::asio::streambuf m_recv_buf;

	ThreadSafeQueue<PacketMessage> m_qMsgIn;
	ThreadSafeQueue<PacketMessage> m_qMsgOut;

	uint32_t m_sessionId;
	atomic<uint32_t> m_roomId;
//	std::deque<packet_message> write_msgs;
	//asio::steady_timer m_timer;
	//packet_message read_msg;
};

