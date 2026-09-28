#pragma once
#include <functional>

namespace AGS_Server
{
	void start_server();
	void stop_server();

	const bool is_server_running();
	void add_client_connected_signal(std::function<void()> delegate);
}