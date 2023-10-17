#pragma once
#include "pch.h"
#include <chrono>


using namespace boost;


bool Session::isConnected() const
{
    if ( m_socket.is_open()) {
        return true;
    }

    return false;
}


bool Session::Send(const char* msg)
{
    boost::asio::post(
        m_pIoContext,
        [this, msg]() {
            // 메시지 큐에 넣기
        }
    );
    return false;
}


bool Session::Start(uint32_t id)
{
    m_sessionId = id;

    asio::async_read(
        m_socket, 
        asio::buffer(m_qMsgIn.Front().data(), PacketMessage::header_length),
        bind(&Session::handleRead, shared_from_this(),
            asio::placeholders::error,
            asio::placeholders::bytes_transferred)
    );

    return true;
}

void Session::handleRead(const boost::system::error_code& error,
    size_t bytes_transferred)
{
    // 패킷헤더 읽고 Flatbuffer로
    if (!error)
    {
        std::cout << "[" << m_sessionId << "] Read Header Fail.\n";
        m_socket.close();
        // TODO: 소켓 삭제
        return;
    }

    PacketMessage packet = m_qMsgIn.PopFront();
    if (packet.decode_header() ==  false)
    {
        std::cout << "[" << m_sessionId << "] Read Header Fail.\n";
        m_socket.close();
        // TODO: 소켓 삭제
        return;
    }

    auto self(shared_from_this());
    asio::async_read(
        m_socket,
        asio::buffer(packet.body(), packet.body_length()  ),
        [this, self](boost::system::error_code& error,
            size_t bytes_transferred) {
            if (!error)
            {
                std::cout << "[" << m_sessionId << "] Read Body Fail.\n";
                m_socket.close();
                // TODO: 소켓 삭제
                return;
            }


        }
    );

    asio::async_read(
        m_socket,
        asio::buffer(m_qMsgIn.Front().data(), PacketMessage::header_length),
        bind(&Session::handleRead, shared_from_this(),
            asio::placeholders::error,
            asio::placeholders::bytes_transferred)
    );
    printf("read data\n");
}

Session::Session(boost::asio::io_context& io_context, boost::asio::ip::tcp::socket socket)
    : m_pIoContext(io_context),
    m_socket(std::move(socket)),
    m_qMsgIn(5),
    m_qMsgOut(20)
{

}

void Session::handleWrite(const boost::system::error_code& error,
    size_t bytes_transferred)
{
    printf("write data\n");
    std::cout << "error:" << error << "  size:" << bytes_transferred << std::endl;
    return;
}