#include "AGS_CDB.h"
#include <mutex>

/*
NOTE:
This class would be better suited as a simple read/write class with no descrimination over Input or Output...
*/

namespace AGS_Server
{
	std::mutex mtx;
}

AGS_Server::CDB::CDB(Config_Rec& config)
{
	std::vector<Label_Data>::iterator itr;

	for (itr = config.input_labels.begin(); itr != config.input_labels.end(); itr++)
		add_input_single_label(itr->label_name, itr->label_type);

	for (itr = config.input_array_labels.begin(); itr != config.input_array_labels.end(); itr++)
		add_input_arr_label(itr->label_name);

	for (itr = config.output_labels.begin(); itr != config.output_labels.end(); itr++)
		add_output_single_label(itr->label_name, itr->label_type);

	for (itr = config.output_array_labels.begin(); itr != config.output_array_labels.end(); itr++)
		add_output_arr_label(itr->label_name);
}

void AGS_Server::CDB::add_listener_update(std::function<void()> event_listener)
{
	update_delegates.push_back(event_listener);
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


bool AGS_Server::CDB::write_input(const AGS_DATA_RECORD& record)
{
	std::lock_guard<std::mutex> lock(mtx);
	bool retval = false;

	std::unordered_map<std::string, AGS_DATA_RECORD>::iterator itr;

	std::string lbl_name = record.label_name;
	itr = input_labels.find(lbl_name);
	if (itr != input_labels.end())
	{
		if (itr->second.data_type == record.data_type)
		{
			itr->second = record;
			retval = true;
		}		
	}
	return retval;
}

bool AGS_Server::CDB::write_arr_input(const AGS_DATA_ARR_RECORD& record)
{
	std::lock_guard<std::mutex> lock(mtx);
	bool retval = false;

	std::unordered_map<std::string, AGS_DATA_ARR_RECORD>::iterator itr;
	std::string lbl_name = record.label_name;
	itr = input_arr_labels.find(lbl_name);
	if (itr != input_arr_labels.end())
	{
		itr->second = record;
		retval = true;
	}

	return retval;
}

bool AGS_Server::CDB::read_input(const std::string& label_name, AGS_DATA_RECORD& value)
{
	std::lock_guard<std::mutex> lock(mtx);
	bool retval = false;

	std::unordered_map<std::string, AGS_DATA_RECORD>::iterator itr;

	itr = input_labels.find(label_name);
	if (itr != input_labels.end())
	{
		value = itr->second;
		retval = true;
	}

	return retval;
}

bool AGS_Server::CDB::read_arr_input(const std::string& label_name, AGS_DATA_ARR_RECORD& value)
{
	std::lock_guard<std::mutex> lock(mtx);
	bool retval = false;

	std::unordered_map<std::string, AGS_DATA_ARR_RECORD>::iterator itr;

	itr = input_arr_labels.find(label_name);
	if (itr != input_arr_labels.end())
	{
		value = itr->second;
		retval = true;
	}

	return retval;
}

bool AGS_Server::CDB::write_output(const AGS_DATA_RECORD& record)
{
	std::lock_guard<std::mutex> lock(mtx);
	bool retval = false;

	std::unordered_map<std::string, AGS_DATA_RECORD>::iterator itr;

	std::string lbl_name = record.label_name;
	itr = output_labels.find(lbl_name);
	if (itr != output_labels.end())
	{
		if (itr->second.data_type == record.data_type)
		{
			itr->second = record;
			retval = true;
		}
	}
	return retval;
}

bool AGS_Server::CDB::write_arr_output(const AGS_DATA_ARR_RECORD& record)
{
	std::lock_guard<std::mutex> lock(mtx);
	bool retval = false;

	std::unordered_map<std::string, AGS_DATA_ARR_RECORD>::iterator itr;

	std::string lbl_name = record.label_name;
	itr = output_arr_labels.find(lbl_name);
	if (itr != output_arr_labels.end())
	{
		itr->second = record;
		retval = true;
	}
	return retval;
}

bool AGS_Server::CDB::read_output(const std::string& label_name, AGS_DATA_RECORD& value)
{
	std::lock_guard<std::mutex> lock(mtx);
	bool retval = false;

	std::unordered_map<std::string, AGS_DATA_RECORD>::iterator itr;

	itr = output_labels.find(label_name);
	if (itr != output_labels.end())
	{
		value = itr->second;
		retval = true;
	}

	return retval;
}

bool AGS_Server::CDB::read_arr_output(const std::string& label_name, AGS_DATA_ARR_RECORD& value)
{
	std::lock_guard<std::mutex> lock(mtx);
	bool retval = false;

	std::unordered_map<std::string, AGS_DATA_ARR_RECORD>::iterator itr;

	itr = output_arr_labels.find(label_name);
	if (itr != output_arr_labels.end())
	{
		value = itr->second;
		retval = true;
	}

	return retval;
}