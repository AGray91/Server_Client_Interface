#pragma once
#include "Config.h"
#include <string>
#include "AGS_Server/AGS_Server.h"
#include <map>

namespace AGS_Server
{
	void initialise_CDB(const Config_Rec* config_data);

	void add_input_single_label(const std::string label_name, const unsigned int label_type, const std::string description);
	void add_input_array_label(const std::string label_name, const unsigned int label_type, const unsigned int quantity, const std::string description);
	void add_output_single_label(const std::string label_name, const unsigned int label_type, const std::string description);
	void add_output_array_label(const std::string label_name, const unsigned int label_type, const unsigned int quantity, const std::string description);

	const std::map<std::string, AGS_DATA_RECORD*>& get_input_single_labels();
	const std::map<std::string, std::vector<char>>& get_input_arr_labels();
	const std::map<std::string, AGS_DATA_RECORD*>& get_output_single_labels();
	const std::map<std::string, std::vector<char>>& get_output_arr_labels();

	bool check_label_valid(const std::string label);

	/* Get Label Type... Returns true if label is found in CDB. Type will be given based on data types found in AGS_Server.h */
	/* Direction: 1 = Input, 2 = Output...*/
	bool get_label_type(const std::string& label_name, int& type);
	bool get_label_type(const std::string& label_name, int& type, int& direction);

	bool get_input_single_label(std::pair<std::string, AGS_DATA_RECORD*>& pair, const std::string label_name);
	bool get_input_arr_label(std::pair<std::string, std::vector<char>>& pair, const std::string label_name);
	bool get_output_single_label(std::pair<std::string, AGS_DATA_RECORD*>& pair, const std::string label_name);
	bool get_output_arr_label(std::pair<std::string, std::vector<char>>& pair, const std::string label_name);

	void register_call_back(void (*func)(const std::string& label_name, const int label_type));
	void call_update_label(const std::string& label_name, const int label_type);
}