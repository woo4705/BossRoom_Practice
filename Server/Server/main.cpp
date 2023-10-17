#include "pch.h"
int main() {
	try {
		//boost::asio::io_context io_context;
		GameServer server(11000);

		//io_context.run();
		server.Start();
		while (1)
		{
			server.Update(-1, true);
		}


	}
	catch (std::exception e) {
		std::cerr << e.what();
	}

	return 0;
}