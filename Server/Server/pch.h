#pragma once

#define BOOST_BIND_GLOBAL_PLACEHOLDERS

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <string>
#include <set>
#include <vector>
#include <queue>
#include <deque>
#include <unordered_map>
#include <mutex>
#include <condition_variable>

#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <boost/asio/async_result.hpp>
#include <boost/asio/buffer.hpp>

#include "./Packet/Packet_generated.h"
#include "ThreadSafeQueue.h"
#include "Session.h"
#include "Lobby.h"
#include "PacketMessage.hpp"
#include "PacketManager.h"
#include "GameServer.h"