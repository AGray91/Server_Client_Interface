#include "Network.h"
#include <WinSock2.h>
#include <WS2tcpip.h>
#include <mutex>

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#endif

namespace AGS_Server
{
	long dead_band = 0.0f;
	unsigned int port_number = 0;
	bool is_network_open = false;
	std::atomic<bool> is_client_connected = false;

	addrinfo* addr_info = nullptr;
	SOCKET listen_socket = INVALID_SOCKET;
	SOCKET client_socket = INVALID_SOCKET;

	DWORD timeout_ms = 1000;

	bool open_network();

	bool start_winsock();
	bool get_local_address();
	bool create_socket();
	bool bind_socket_to_addr();
	bool set_socket_to_listen();
}

bool AGS_Server::initialise_network(unsigned int _port_number, long _dead_band)
{
	port_number = _port_number;
	dead_band = _dead_band;
	return open_network();
}

bool AGS_Server::open_network()
{
	bool open_status = false;

	open_status = start_winsock();
	open_status = get_local_address();
	open_status = create_socket();
	open_status = bind_socket_to_addr();
	open_status = set_socket_to_listen();

	is_network_open = open_status;

	if (!is_network_open)
		close_network();

	return is_network_open;
}

void AGS_Server::close_network()
{
	is_network_open = false;

	if (addr_info != nullptr)
	{
		freeaddrinfo(addr_info);
		addr_info = nullptr;
	}

	if (is_client_connected)
	{
		if (client_socket != INVALID_SOCKET)
			closesocket(client_socket);
	}

	if (listen_socket != INVALID_SOCKET)
		closesocket(listen_socket);

	WSACleanup();
}

// Waots for a client connection and then sets new socket as the client socket...
bool AGS_Server::wait_for_connection()
{
	bool retval = false;

	fd_set read_set;
	FD_ZERO(&read_set);
	FD_SET(listen_socket, &read_set);

	timeval tv{};
	tv.tv_sec = dead_band;	// Time in seconds. How long to wait for a connection before cycling again...
	tv.tv_usec = 0;

	int ready = select(0, &read_set, nullptr, nullptr, &tv);

	if (ready > 0 && FD_ISSET(listen_socket, &read_set))
	{
		// Log Waiting for incoming signals from client...
		client_socket = accept(listen_socket, NULL, NULL);
		// Log Signal accepted...

		if (client_socket != INVALID_SOCKET)
		{
			// Log Client connection established...
			retval = true;
		}
		else
		{
			int err = WSAGetLastError();

			if (err == WSAETIMEDOUT)
			{
				// Log connection timed out...
				retval = true;
			}
			else
			{
				// Log accept() failed with error...
			}
		}
	}
	else
	{
		// Log no connection...
	}

	is_client_connected = retval;
	return retval;
}

bool AGS_Server::get_is_network_open()
{
	return is_network_open;
}

bool AGS_Server::get_is_client_connected()
{
	return is_client_connected;
}

std::string AGS_Server::get_ip_address()
{
	return std::string();
}

bool AGS_Server::receive_msg(char* buffer, int length)
{
	bool retval = false;

	unsigned int total_received = 0;
	
	while (total_received < length)
	{
		int bytes_received = recv(client_socket, buffer + total_received, length - total_received, 0);

		if (bytes_received == SOCKET_ERROR)
		{
			int err = WSAGetLastError();

			if (err = WSAETIMEDOUT)
			{
				// Log timeout...
				continue;
			}
			else
			{
				retval = false;
				is_client_connected = retval;
				// Log error...
			}

			break;
		}
		else if (bytes_received == 0)
		{
			// Log received 0 bytes...

			retval = true;
			is_client_connected = retval;
			break;
		}
		else
		{
			total_received += bytes_received;
			retval = true;
		}
	}

	return retval;
}

bool AGS_Server::send_msg(const char* buffer, int length)
{
	// Log sending data...

	bool retval = false;

	int total_sent = 0;

	while (total_sent < length)
	{
		int bytes_sent = send(client_socket, buffer + total_sent, length - total_sent, 0);

		if (bytes_sent == SOCKET_ERROR)
		{
			// Log error...

			retval = false;
			break;
		}
		else if (bytes_sent == 0)
		{
			retval = true;
			break;
		}
		else
		{
			total_sent += bytes_sent;
			retval = true;
		}
	}

	if (retval)
	{
		// Log data sent successfully...
	}
	
	is_client_connected = retval;
	return retval;
}

bool AGS_Server::start_winsock()
{
	bool retval = false;

	WSADATA wsadata;
	int result = WSAStartup(MAKEWORD(2, 2), &wsadata);

	if (result == 0)
		retval = true;

	return retval;
}

bool AGS_Server::get_local_address()
{
	bool retval = false;

	addrinfo hints;
	ZeroMemory(&hints, sizeof(hints));

	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;
	hints.ai_flags = AI_PASSIVE;

	// Resolve the local address and port to be used by the server...
	int result = getaddrinfo(NULL, std::to_string(port_number).data(), &hints, &addr_info);

	if (result == 0)
	{
		char host[NI_MAXHOST];
		getnameinfo(addr_info->ai_addr, addr_info->ai_addrlen, host, NI_MAXHOST, nullptr, 0, NI_NUMERICHOST);
		retval = true;
	}
	else
	{
		// Log error...
	}

	return retval;
}

bool AGS_Server::create_socket()
{
	bool retval = false;

	listen_socket = socket(addr_info->ai_family, addr_info->ai_socktype, addr_info->ai_protocol);

	if (listen_socket != INVALID_SOCKET)
	{
		// Log succesful creation of socket...
		retval = true;
	}
	else
	{
		// Log error...
	}

	return retval;
}

bool AGS_Server::bind_socket_to_addr()
{
	bool retval = false;

	int result = bind(listen_socket, addr_info->ai_addr, (int)addr_info->ai_addrlen);
	if (result != SOCKET_ERROR)
	{
		// Log successful socket binding to address and port...
		freeaddrinfo(addr_info);
		addr_info = nullptr;
		retval = true;
	}
	else
	{
		// Log error...
	}

	return retval;
}

bool AGS_Server::set_socket_to_listen()
{
	bool retval = false;

	int result = listen(listen_socket, SOMAXCONN);
	if (result != SOCKET_ERROR)
	{
		retval = true;
		// Log success..
	}
	else
	{
		// Log error...
	}

	return retval;
}