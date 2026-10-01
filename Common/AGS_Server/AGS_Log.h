#pragma once
#include <string>

namespace AGS_Server
{
	// Must initialise log before use...
	bool initialise_log();

	void log(std::string& msg);
	void log_stream(const char* msg, int length);

	// Must shut down log after use...
	void shutdown_log();
}