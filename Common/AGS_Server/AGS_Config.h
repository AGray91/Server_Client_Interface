#pragma once
#include <vector>
#include <string>

namespace AGS_Server
{
	struct Label_Data
	{
		std::string label_name;
		unsigned int label_type;
		unsigned int quantity;
		std::string description;
	};

	struct Config_Rec
	{
		bool server_connect;
		unsigned int server_port;
		std::vector<Label_Data> input_labels;
		std::vector<Label_Data> input_array_labels;
		std::vector<Label_Data> output_labels;
		std::vector<Label_Data> output_array_labels;
	};

	// Gives output struct all config data found in config file...
	bool read_config_file(Config_Rec& output);
}