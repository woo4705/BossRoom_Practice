//
// PacketMessage.hpp
// ~~~~~~~~~~~~~~~~
//
// Copyright (c) 2003-2022 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#pragma once
#include "pch.h"

#ifndef PACKET_MESSAGE_HPP
#define PACKET_MESSAGE_HPP


class PacketMessage
{
public:
    static constexpr std::size_t header_type_info_length = 4;
    static constexpr std::size_t header_size_info_length = 4;
    static constexpr std::size_t header_length = 8; // ( header_type_info_length + header_size_info_length )
    static constexpr std::size_t max_body_length = 1024;

    PacketMessage()
        : m_bodyLength(0),
        m_data()
    {
    }

    const uint8_t* data() const
    {
        return m_data;
    }

    uint8_t* data()
    {
        return m_data;
    }

    std::size_t length() const
    {
        return header_length + m_bodyLength;
    }

    const uint8_t* body() const
    {
        return m_data + header_length;
    }

    uint8_t* body()
    {
        return m_data + header_length;
    }

    std::size_t body_length() const
    {
        return m_bodyLength;
    }

    void body_length(std::size_t new_length)
    {
        m_bodyLength = new_length;
        if (m_bodyLength > max_body_length)
            m_bodyLength = max_body_length;
    }

    bool decode_header()
    {
        char header[header_length + 1] = "";
        strncat_s(header, (char*)m_data, header_length);
        m_bodyLength = std::atoi(header);
        if (m_bodyLength > max_body_length)
        {
            m_bodyLength = 0;
            return false;
        }
        return true;
    }

    void encode_header()
    {
        char header[header_length + 1] = "";
        printf(header, "%4d", static_cast<int>(m_bodyLength));
        std::memcpy(m_data, header, header_length);
    }

private:
    uint8_t m_data[header_length + max_body_length];
    std::size_t m_bodyLength;
};

#endif // PACKET_MESSAGE_HPP