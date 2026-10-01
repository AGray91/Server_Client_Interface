#include "AGS_Log.h"
#include <fstream>
#include <ctime>
#include <sstream>
#include <iomanip>

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#endif


namespace AGS_Server
{
	std::ofstream logger;
	std::ofstream stream_logger;

	int byte_number = 0;

	std::string get_date_time();
	std::string get_date_time_for_file();
}

bool AGS_Server::initialise_log()
{
	bool retval = false;
	CreateDirectoryA("Log", NULL);

	std::string date_opened = get_date_time_for_file();
	std::string log_file_name = "Log/AGS_Server_Log_" + date_opened + ".txt";

	// NOTE: This currently clogs up the Log folder indefinitely by adding new log files each time...

	logger.open(log_file_name);

	if (logger.is_open())
	{
		logger << get_date_time() << ": Log file opened..." << std::endl;
		retval = true;
	}

#ifdef DEBUG
	std::string stream_log_file_name = "Log/AGS_Server_Stream_Log_" + date_opened + ".txt";
	stream_logger.open(stream_log_file_name);

	if (stream_logger.is_open())
	{
		stream_logger << "AGS_Server Stream Log opened..." << std::endl;
		stream_logger << "** BYTE NUMBER ** DECIMAL ** ASCII ** ANNOTATION **" << std::endl;
		retval = true;
	}
	else
	{
		retval = false;
#endif

	return retval;
}

void AGS_Server::shutdown_log()
{
	if (stream_logger.is_open())
		stream_logger.close();

	if (logger.is_open())
		logger.close();
}

void AGS_Server::log(std::string& msg)
{
	if (!logger.is_open())
		return;

	logger << get_date_time() << ": " << msg.data() << std::endl;
}

void AGS_Server::log_stream(const char* msg, int length)
{
	if (!stream_logger.is_open())
		return;

	for (int i = 0; i < length; i++)
	{
		unsigned int c = static_cast<unsigned int>(static_cast<unsigned char>(msg[i]));
		stream_logger << "BYTE[" << byte_number << "]\t\t\t" << c << " \t\t " << msg[i] << std::endl;
		byte_number++;
	}
}

std::string AGS_Server::get_date_time()
{
	std::string retval = "";

	std::time_t now = std::time(nullptr);

	if (now == -1)
	{
		retval = "Failed to get current time...";
	}
	else
	{
		std::tm local_time{};
		localtime_s(&local_time, &now);
		std::ostringstream oss;
		oss << std::put_time(&local_time, "%d/%m/%Y %H:%M:%S");

		retval = oss.str();
	}

	return retval;
}

std::string AGS_Server::get_date_time_for_file()
{
	std::string retval = "";

	std::time_t now = std::time(nullptr);

	if (now == -1)
	{
		retval = "Failed to get current time...";
	}
	else
	{
		std::tm local_time{};
		localtime_s(&local_time, &now);
		std::ostringstream oss;
		oss << std::put_time(&local_time, "%d_%m_%Y_%H_%M_%S");

		retval = oss.str();
	}

	return retval;
}