#include "AGS_CDB.h"
#include <mutex>

namespace AGS_Server
{
	std::mutex mtx;
}

AGS_Server::CDB::CDB()
{

}

void AGS_Server::CDB::add_listener_update(std::function<void> event_listener)
{
	update_delegates.push_back(std::move(event_listener));
}

// Create a new entry in the Input Single Label CDB...
void AGS_Server::CDB::add_input_single_label(const std::string label_name, const unsigned int label_type)
{
	std::pair<std::string, AGS_DATA_RECORD> new_label;
	new_label.first = label_name;
	new_label.second = AGS_DATA_RECORD();
	new_label.second.data_type = label_type;
	strncpy_s(new_label.second.label_name, label_name.c_str(), MAX_LABELNAME_SIZE);

	input_labels.insert(new_label);
}

// Create a new entry in the Input Array Label CDB...
void AGS_Server::CDB::add_input_arr_label(const std::string label_name)
{
	std::pair<std::string, AGS_DATA_ARR_RECORD> new_arr_label;
	new_arr_label.first = label_name;
	new_arr_label.second = AGS_DATA_ARR_RECORD();
	strncpy_s(new_arr_label.second.label_name, label_name.c_str(), MAX_LABELNAME_SIZE);

	input_arr_labels.insert(new_arr_label);
}

// Create a new entry in the Output Single Label CDB...
void AGS_Server::CDB::add_output_single_label(const std::string label_name, const unsigned int label_type)
{
	std::pair<std::string, AGS_DATA_RECORD> new_label;
	new_label.first = label_name;
	new_label.second = AGS_DATA_RECORD();
	new_label.second.data_type = label_type;
	strncpy_s(new_label.second.label_name, label_name.c_str(), MAX_LABELNAME_SIZE);

	output_labels.insert(new_label);
}

// Create a new entry in the Output Array Label CDB...
void AGS_Server::CDB::add_output_arr_label(const std::string label_name)
{
	std::pair<std::string, AGS_DATA_ARR_RECORD> new_arr_label;
	new_arr_label.first = label_name;
	new_arr_label.second = AGS_DATA_ARR_RECORD();
	strncpy_s(new_arr_label.second.label_name, label_name.c_str(), MAX_LABELNAME_SIZE);

	output_arr_labels.insert(new_arr_label);
}

// Get all label names for single input labels...
const std::vector<std::string> AGS_Server::CDB::get_input_single_labels()
{
	std::vector<std::string> retval;

	std::unordered_map<std::string, AGS_DATA_RECORD>::iterator itr;

	for (itr = input_labels.begin(); itr != input_labels.end(); itr++)
	{
		retval.push_back(itr->first);
	}

	return retval;
}

// Get all label names for single array labels...
const std::vector<std::string> AGS_Server::CDB::get_input_arr_labels()
{
	std::vector<std::string> retval;

	std::unordered_map<std::string, AGS_DATA_ARR_RECORD>::iterator itr;

	for (itr = input_arr_labels.begin(); itr != input_arr_labels.end(); itr++)
	{
		retval.push_back(itr->first);
	}

	return retval;
}

// Get all label names for single output labels...
const std::vector<std::string> AGS_Server::CDB::get_output_single_labels()
{
	std::vector<std::string> retval;

	std::unordered_map<std::string, AGS_DATA_RECORD>::iterator itr;

	for (itr = output_labels.begin(); itr != output_labels.end(); itr++)
	{
		retval.push_back(itr->first);
	}

	return retval;
}

// Get all label names for array output labels...
const std::vector<std::string> AGS_Server::CDB::get_output_arr_labels()
{
	std::vector<std::string> retval;

	std::unordered_map<std::string, AGS_DATA_ARR_RECORD>::iterator itr;

	for (itr = output_arr_labels.begin(); itr != output_arr_labels.end(); itr++)
	{
		retval.push_back(itr->first);
	}

	return retval;
}

// write_input INT1...
bool AGS_Server::CDB::write_input(const std::string& label_name, const char value)
{
	std::lock_guard<std::mutex> lock(mtx);
	bool retval = false;

	std::unordered_map<std::string, AGS_DATA_RECORD>::iterator itr;
	
	itr = input_labels.find(label_name);
	if (itr != input_labels.end())
	{
		if (itr->second.data_type = AGS_DATATYPE_SINT8)
		{
			itr->second.data.s8 = value;
			retval = true;
		}
	}

	return retval;
}

// Write_input LOG1
bool AGS_Server::CDB::write_input(const std::string& label_name, const unsigned char value)
{
	std::lock_guard<std::mutex> lock(mtx);
	bool retval = false;

	std::unordered_map<std::string, AGS_DATA_RECORD>::iterator itr;

	itr = input_labels.find(label_name);
	if (itr != input_labels.end())
	{
		if (itr->second.data_type = AGS_DATATYPE_UINT8)
		{
			itr->second.data.u8 = value;
			retval = true;
		}
	}

	return retval;
}

bool AGS_Server::CDB::write_input(const std::string& label_name, const short value)
{
	return false;
}

bool AGS_Server::CDB::write_input(const std::string& label_name, const unsigned short value)
{
	return false;
}

bool AGS_Server::CDB::write_input(const std::string& label_name, const int value)
{
	return false;
}

bool AGS_Server::CDB::write_input(const std::string& label_name, const unsigned int value)
{
	return false;
}

bool AGS_Server::CDB::write_input(const std::string& label_name, const float value)
{
	return false;
}

bool AGS_Server::CDB::write_input(const std::string& label_name, const double value)
{
	return false;
}

bool AGS_Server::CDB::write_input(const std::string& label_name, const char* value)
{
	return false;
}

bool AGS_Server::CDB::read(const std::string& label_name, char& value)
{
	return false;
}

bool AGS_Server::CDB::read(const std::string& label_name, unsigned char& value)
{
	return false;
}

bool AGS_Server::CDB::read(const std::string& label_name, short& value)
{
	return false;
}

bool AGS_Server::CDB::read(const std::string& label_name, unsigned short& value)
{
	return false;
}

bool AGS_Server::CDB::read(const std::string& label_name, int& value)
{
	return false;
}

bool AGS_Server::CDB::read(const std::string& label_name, unsigned int& value)
{
	return false;
}

bool AGS_Server::CDB::read(const std::string& label_name, float& value)
{
	return false;
}

bool AGS_Server::CDB::read(const std::string& label_name, double& value)
{
	return false;
}

bool AGS_Server::CDB::read(const std::string& label_name, char* value)
{
	return false;
}
