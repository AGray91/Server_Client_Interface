#include "Server.h"
#include <vector>
#include <thread>

namespace AGS_Server
{
	bool server_running = false;
	bool client_connected = false;

	std::vector<std::function<void()>> client_connected_signals;
}

void AGS_Server::start_server()
{
	server_running = true;
	std::thread send_loop_thread;

	while (server_running)
	{
		// If get_is_client_connected()
	}
}

void AGS_Server::stop_server()
{
	server_running = false;
}

const bool AGS_Server::is_server_running()
{
	return server_running;
}

void AGS_Server::add_client_connected_signal(std::function<void()> delegate)
{

}
