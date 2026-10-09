#include "Server.h"
#include <thread>
#include "AGS_Server/AGS_Server.h"
#include "AGS_Server/AGS_Network.h"
#include "AGS_Server/AGS_Log.h"
#include <winsock.h>
#include "AGS_Server/AGS_CDB.h"

namespace AGS_Server
{
	bool server_running = false;
	bool client_has_connected = false;

	std::vector<std::function<void()>> client_connected_signals;

	CDB* cdb;
	Config_Rec* config;

	AGS_DATA_PACKET in_payload;
	AGS_DATA_PACKET out_payload;

	void send_loop();
	void send_client_connected();
	void receive_from_client();
	void send_to_client();

	// bool deserialise_header(const char* input, AGS_Header& output);
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

	delete config;
	delete cdb;

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

	uint8_t header[9];
	if (receive_msg(header, sizeof(header)))
	{
		memcpy_s(&in_payload.header, sizeof(in_payload.header), header, sizeof(in_payload.header));
		memcpy_s(&in_payload.message_type, sizeof(in_payload.message_type), header + 4, sizeof(in_payload.message_type));
		memcpy_s(&in_payload.num_records, sizeof(in_payload.num_records), header + 5, sizeof(in_payload.num_records));

		if (in_payload.header != 0x5A5A5A5A)
		{
			msg = "Failed to receive correct header...";
			log(msg);

			stop_server();
			return;
		}

		uint8_t* payload = new uint8_t[sizeof(AGS_DATA_RECORD) * in_payload.num_records];

		msg = "Reading payload...";
		log(msg);

		if (receive_msg(payload, sizeof(payload)))
		{
			switch (in_payload.message_type)
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

	/*
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
	*/
}

void AGS_Server::send_to_client()
{
	if (cdb == nullptr)
		return;

	// Header code and message type...
	out_payload.header = 0x5A5A5A5A;
	out_payload.message_type = AGS_MSGTYPE_WRITETOCLIENT;

	// Update out_payload...
	out_payload.num_records = config->output_labels.size();
	out_payload.records.clear();
	out_payload.records.reserve(out_payload.num_records);

	AGS_DATA_RECORD new_rec;

	// Loop through each output label...
	for (const Label_Data& label : config->output_labels)
	{
		cdb->read_output(label.label_name, new_rec);	// Get data for label from CDB using label name...
		out_payload.records.push_back(new_rec);
	}

	// Serialise out_payload...
	std::vector<uint8_t> buffer;

	// Calculate the total size of the out_payload in advance..
	size_t total_bytes = sizeof(out_payload.header) + sizeof(out_payload.message_type) + sizeof(out_payload.num_records);
	total_bytes += out_payload.records.size() * (MAX_LABELNAME_SIZE + sizeof(char) + sizeof(AGS_LABEL_VALUE));

	// Reserve memory for buffer...
	buffer.clear();
	buffer.reserve(total_bytes);

	// Serialise out_payload into buffer...
	serialise(buffer, out_payload.header);
	serialise(buffer, out_payload.message_type);
	serialise(buffer, out_payload.num_records);

	for (const AGS_DATA_RECORD& record : out_payload.records)
	{
		serialise(buffer, record.label_name);
		serialise(buffer, record.data_type);
		serialise(buffer, record.data);
	}

	// Send data over network...
	send_msg(buffer.data(), buffer.size());
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

// Function to set a ptr to the config currently informing the server. Returns true if the config is valid...
bool AGS_Server::get_server_config(AGS_Server::Config_Rec** ptr)
{
	bool retval = false;

	if (config != nullptr)
	{
		retval = true;
		*ptr = config;
	}

	return retval;
}

/*
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
*/