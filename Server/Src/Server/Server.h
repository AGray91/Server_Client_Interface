#pragma once
#include <functional>
#include <vector>
#include "AGS_Server/AGS_Config.h"

#define SEND_RATE	1000

namespace AGS_Server
{
	void start_server();
	void stop_server();

	const bool is_server_running();
	void add_client_connected_signal(std::function<void()> _delegate);

	// Function to set a ptr to the config currently informing the server. Returns true if the config is valid...
	bool get_server_config(Config_Rec** ptr);

	template<typename T>
	void serialise(std::vector<uint8_t>& buffer, const T& value)
	{
		const uint8_t* ptr = reinterpret_cast<const uint8_t*>(&value);
		buffer.insert(buffer.end(), ptr, ptr + sizeof(T));
	}

	template<typename T>
	T deserialise(const uint8_t& ptr)
	{
		T value;
		std::memcpy(&value, ptr, sizeof(T));
		ptr += sizeof(T);
		return value;
	}
}