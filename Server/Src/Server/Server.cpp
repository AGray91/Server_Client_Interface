#include "Server.h"
#include <thread>
#include "AGS_Server/AGS_Network.h"
#include "AGS_Server/AGS_Log.h"
#include <winsock.h>
#include "AGS_Server/AGS_Server.h"
#include "AGS_Server/AGS_CDB.h"
#include "AGS_Server/AGS_Config.h"

namespace AGS_Server
{
	struct AGS_Header
	{
		int32_t payload_size;
		int32_t msg_type;
		int32_t client_id;
	};

	bool server_running = false;
	bool client_has_connected = false;

	std::vector<std::function<void()>> client_connected_signals;

	CDB* cdb;
	Config_Rec* config;

	void send_loop();
	void send_client_connected();
	void receive_from_client();
	void send_to_client();

	bool deserialise_header(const char* input, AGS_Header& output);
}

void AGS_Server::start_server()
{
	server_running = true;
	std::thread send_loop_thread;

	// Initialise data from config file...
	config = new Config_Rec();
	if (!read_config_file(*config))
	{
		server_running = false;
		std::string msg = "Failed to read config file!";
		log(msg);
	}

	// Initialise a new CDB with config data...
	cdb = new CDB(*config);

	while (server_running)
	{
		if (get_is_client_connected())
		{
			if (!client_has_connected)
			{
				send_client_connected();
				client_has_connected = true;
				send_loop_thread = std::thread(send_loop);
			}

			receive_from_client();
		}
		else
		{
			if (client_has_connected)
			{
				send_client_connected();
				client_has_connected = false;
			}

			if (send_loop_thread.joinable())
				send_loop_thread.join();

			wait_for_connection();
		}
	}

	if (send_loop_thread.joinable())
		send_loop_thread.join();

	shutdown_log();
	close_network();
}

void AGS_Server::stop_server()
{
	server_running = false;
}

// Function that sits in it's own thread and continuously sends all label data register for by a client...
void AGS_Server::send_loop()
{
	std::string msg = "Starting Send Loop...";
	log(msg);

	while (get_is_client_connected())
	{
		std::this_thread::sleep_for(std::chrono::milliseconds(SEND_RATE));
		send_to_client();
	}

	msg = "Send Loop Finishing...";
	log(msg);
}

void AGS_Server::receive_from_client()
{
	std::string msg = "Reading from client...";
	log(msg);

	char header[16];
	char* payload;

	msg = "Reading header...";
	log(msg);

	if (receive_msg(header, sizeof(header)))
	{
		AGS_Header header_data;
		if (!deserialise_header(header, header_data))
		{
			msg = "Failed to deserialise header...";
			log(msg);

			stop_server();
			return;
		}

		payload = new char[header_data.payload_size];

		msg = "Reading payload...";
		log(msg);

		if (receive_msg(payload, header_data.payload_size))
		{
			msg = "Payload type: " + std::to_string(header_data.msg_type);
			log(msg);

			switch (header_data.msg_type)
			{
			case AGS_MSGTYPE_REGISTER:
				break;

			case AGS_MSGTYPE_WRITETOSERVER:
				break;

			case AGS_MSGTYPE_WRITEARRAYTOSERVER:
				break;
			}
		}
		else
		{
			msg = "Failed to recieve message when looking for payload...";
			log(msg);
		}

		delete[] payload;
	}
	else
	{
		msg = "Failed to receive message when looking for header...";
		log(msg);
	}

	msg = "Finished reading from client...";
	log(msg);
}

void AGS_Server::send_to_client()
{
	if (cdb == nullptr)
		return;

	// Create header...
	AGS_Header header;

	// Get all single labels...
	std::vector<std::string> output_label_names = cdb->get_output_single_labels();

	// Create payload...
	std::vector<char> payload;

	// Get number of labels...
	append_to_buff(payload, output_label_names.size());

	// Loop through each output label...
	for (size_t i = 0; i < output_label_names.size(); i++)
	{
		// Add label name...
		char label[MAX_LABELNAME_SIZE];
		ZeroMemory(label, MAX_LABELNAME_SIZE);
		memcpy_s((void*)label, MAX_LABELNAME_SIZE, &output_label_names[i], MAX_LABELNAME_SIZE);

		// Add label type..
		// NOTE: this is proving harder to be generic... Need a way to get data information without simply copying out the whole database...
		// Perhaps CDB needs to be more flexible, rather than just allowing read/write??
	}
}

const bool AGS_Server::is_server_running()
{
	return server_running;
}

void AGS_Server::add_client_connected_signal(std::function<void()> _delegate)
{
	client_connected_signals.push_back(_delegate);
}

void AGS_Server::send_client_connected()
{
	for (size_t i = 0; i < client_connected_signals.size(); i++)
	{
		client_connected_signals[i]();
	}
}

bool AGS_Server::deserialise_header(const char* input, AGS_Header& output)
{
	bool retval = false;

	std::string input_as_string = input;

	std::string header = "ZZZZ";
	uint32_t size = 0;
	uint32_t msg_type = 0;
	uint32_t client_id = 0;

	// Extract data from input into allocated buffers. Used Hardcoded offsets as these will never change...
	if (strlen(input) < 1)
		return retval;

	memcpy_s(&size, sizeof(size), input + 4, sizeof(size));
	memcpy_s(&msg_type, sizeof(msg_type), input + 8, sizeof(msg_type));
	memcpy_s(&client_id, sizeof(client_id), input + 12, sizeof(client_id));

	if (input_as_string == header)
	{
		retval = true;
		output.payload_size = ntohl(size) - 8;
		output.msg_type = msg_type;
		output.client_id = client_id;
	}

	return retval;
}