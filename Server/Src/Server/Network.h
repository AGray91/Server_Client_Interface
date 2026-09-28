#pragma once
#include <string>

namespace AGS_Server
{
	// Main start/stop network functions...
	bool initialise_network(unsigned int _port_number, long _dead_band);
	void close_network();
	bool wait_for_connection();

	// Getter functions...
	bool get_is_network_open();
	bool get_is_client_connected();
	std::string get_ip_address();

	// TCP send/receive functions...
	bool receive_msg(char* buffer, int length);
	bool send_msg(const char* buffer, int length);
}