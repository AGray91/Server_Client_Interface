#pragma once
#include <functional>
#include <vector>

#define SEND_RATE	1000

namespace AGS_Server
{
	void start_server();
	void stop_server();

	const bool is_server_running();
	void add_client_connected_signal(std::function<void()> _delegate);

	template<typename T>
	void append_to_buff(std::vector<char>& buffer, const T& value)
	{
		const char* ptr = reinterpret_cast<const char*>(&value);
		buffer.insert(buffer.end(), ptr, ptr + sizeof(T));
	}
}