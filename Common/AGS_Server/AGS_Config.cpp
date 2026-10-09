#include "AGS_Config.h"
#include <fstream>
#include <sstream>
#include <algorithm>
#include "AGS_Server.h"
#include "AGS_Log.h"

namespace AGS_Server
{
	// Section markers in the config file...
	size_t START_OF_LINE = 0;
	const std::string COMMENT_MARKER("#");
	const std::string CONNECT_MARKER("#CONNECT");
	const std::string ADDRESS_MARKER("#SERVER_ADDRESS");
	const std::string PORT_MARKER("#PORT_NUMBER");
	const std::string INPUT_START_MARKER("#CDB_INPUT_START");
	const std::string INPUT_STOP_MARKER("#CDB_INPUT_STOP");
	const std::string OUTPUT_START_MARKER("#CDB_OUTPUT_START");
	const std::string OUTPUT_STOP_MARKER("#CDB_OUTPUT_STOP");

	std::string file_name = "CDB.cfg";
	int port_number = 49301;
	float dead_band = 0.0f;

	bool find_section(const std::string& section_marker, std::ifstream& config_file);
	bool is_section_end(const std::string& line);
	bool is_comment(const std::string& line);
	bool is_whitespace(const std::string& line);
	void trim(std::string& line);

	std::string extract_label_datatype(const std::string& line);
	unsigned int get_single_datatype(const std::string& datatype);
	void extract_single_label_data(const std::string& line, std::string& label_name, std::string& label_type, std::string& description);
	void extract_array_label_data(const std::string& line, std::string& label_name, std::string& label_type, std::string& quantity, std::string& description);

	bool read_server_connect(std::ifstream& config_file);
	unsigned int read_server_port(std::ifstream& config_file);

	std::vector<Label_Data> read_single_labels(std::ifstream& config_file);
	std::vector<Label_Data> read_array_labels(std::ifstream& config_file);
}

bool AGS_Server::read_config_file(AGS_Server::Config_Rec& output)
{
	bool retval = false;

	std::ifstream config_file(file_name, std::ios::binary);
	if (config_file.is_open())
	{
		retval = true;

		// Check if we should connected to Server...
		if (find_section(CONNECT_MARKER, config_file))
		{
			output.server_connect = read_server_connect(config_file);
		}
		else
		{
			// Log error...
			std::string msg = "ERROR: No server connect marker found in config file!";
			log(msg);
		}

		// Get the server port number from config file...
		if (find_section(PORT_MARKER, config_file))
		{
			output.server_port = read_server_port(config_file);
		}
		else
		{
			// Log error...
			std::string msg = "ERROR: No port marker found in config file!";
			log(msg);
		}

		// Gather the single input labels from the config file... NOTE: Input = input to server...
		if (find_section(INPUT_START_MARKER, config_file))
		{
			output.input_labels = read_single_labels(config_file);
		}
		else
		{
			// Log error...
			std::string msg = "ERROR: No input label marker found in config file!";
			log(msg);
		}

		// Gather the input array labels from the config file... NOTE: Input = input to server...
		if (find_section(INPUT_START_MARKER, config_file))
		{
			output.input_array_labels = read_array_labels(config_file);
		}
		else
		{
			// Log error...
			std::string msg = "ERROR: No input label marker found in config file!";
			log(msg);
		}

		// Gather the single output labels from the config file... NOTE: Output = output from server...
		if (find_section(OUTPUT_START_MARKER, config_file))
		{
			output.output_labels = read_single_labels(config_file);
		}
		else
		{
			// Log error...
			std::string msg = "ERROR: No output label marker found in config file!";
			log(msg);
		}


		// Gather the array output labels from the config file... NOTE: Output = output from server...
		if (find_section(OUTPUT_START_MARKER, config_file))
		{
			output.output_array_labels = read_array_labels(config_file);
		}
		else
		{
			// Log error...
			std::string msg = "ERROR: No output label marker found in config file!";
			log(msg);
		}
	}

	return retval;
}

bool AGS_Server::read_server_connect(std::ifstream& config_file)
{
	bool retval = false;
	std::string line;

	while (!retval && getline(config_file, line))
	{
		if (is_section_end(line))
		{
			break;
		}
		else
		{
			if (!is_comment(line) && !is_whitespace(line))
			{
				trim(line);
				std::transform(line.begin(), line.end(), line.begin(), ::toupper);
				bool connect = (line.compare("NO") != 0);
				retval = connect;
			}
		}
	}

	if (retval)
	{
		// Log connection flag...
		std::string msg = "Server Connect Flag: YES";
		log(msg);
	}
	else
	{
		// Log connection flag...
		std::string msg = "Server Connect Flag: NO";
		log(msg);
	}

	return retval;
}

unsigned int AGS_Server::read_server_port(std::ifstream& config_file)
{
	unsigned int retval = 0;
	bool found = false;
	std::string line;

	while (!found && getline(config_file, line))
	{
		if (is_section_end(line))
		{
			break;
		}
		else if (!is_comment(line) && !is_whitespace(line))
		{
			found = true;
			trim(line);
			retval = stoi(line);
		}
	}

	if (found)
	{
		// Log Server Port...
		std::string msg = "Server Port Set: " + line;
		log(msg);
	}
	else
	{
		// Log error..
		std::string msg = "ERROR: No server port found!";
		log(msg);
	}

	return retval;
}

std::vector<AGS_Server::Label_Data> AGS_Server::read_single_labels(std::ifstream& config_file)
{
	std::vector<Label_Data> retval;

	std::string line;
	while (getline(config_file, line))
	{
		if (is_section_end(line))
		{
			break;
		}
		else
		{
			if (!is_comment(line) && !is_whitespace(line))
			{
				std::string label_name;
				std::string label_type;
				std::string quantity;
				std::string description;

				label_type = extract_label_datatype(line);

				if (get_single_datatype(label_type) != AGS_DATATYPE_INVALID)
				{
					extract_single_label_data(line, label_name, label_type, description);

					Label_Data new_label;
					new_label.label_name = label_name;
					new_label.label_type = get_single_datatype(label_type);
					new_label.description = description;
					new_label.quantity = 0;

					retval.push_back(new_label);
				}
			}
		}
	}

	return retval;
}

std::vector<AGS_Server::Label_Data> AGS_Server::read_array_labels(std::ifstream& config_file)
{
	std::vector<Label_Data> retval;

	std::string line;
	while (getline(config_file, line))
	{
		if (is_section_end(line))
		{
			break;
		}
		else
		{
			if (!is_comment(line) && !is_whitespace(line))
			{
				std::string label_name;
				std::string label_type;
				std::string quantity;
				std::string description;

				label_type = extract_label_datatype(line);

				if (get_single_datatype(label_type) == AGS_DATATYPE_INVALID)
				{
					extract_array_label_data(line, label_name, label_type, quantity, description);

					Label_Data new_label;
					new_label.label_name = label_name;
					new_label.label_type = get_single_datatype(label_type);
					new_label.description = description;
					new_label.quantity = std::stoi(quantity);

					retval.push_back(new_label);
				}
			}
		}
	}

	return retval;
}

std::string AGS_Server::extract_label_datatype(const std::string& line)
{
	std::string temp;
	std::string label_data_type;
	std::stringstream splitter;
	splitter << line;

	// Discard the first item in the line...
	splitter >> temp;

	// The data type should be the second item in the line...
	splitter >> label_data_type;
	return label_data_type;
}

void AGS_Server::extract_single_label_data(const std::string& line, std::string& label_name, std::string& label_type, std::string& description)
{
	std::stringstream splitter;
	splitter << line;

	// Extract all the label info from the string...
	splitter >> label_name;
	splitter >> label_type;

	// Now extract the description padding words with a space ' '...
	bool first_pass = true;
	std::string temp;
	while (splitter >> temp)
	{
		if (!first_pass)
		{
			description += " ";
		}
		description += temp;
		first_pass = false;
	}
}

void AGS_Server::extract_array_label_data(const std::string& line, std::string& label_name, std::string& label_type, std::string& quantity, std::string& description)
{
	std::stringstream splitter;
	splitter << line;

	// Extract all the label info from the string...
	splitter >> label_name;
	splitter >> label_type;
	splitter >> quantity;

	// Now extract the description padding words witha a space ' '...
	bool first_pass = true;
	std::string temp;

	while (splitter >> temp)
	{
		if (!first_pass)
		{
			description += " ";
		}
		description += temp;
		first_pass;
	}
}

unsigned int AGS_Server::get_single_datatype(const std::string& data_type)
{
	unsigned int type = AGS_DATATYPE_INVALID;

	try
	{
		if (data_type.compare("STR8") == 0) type = AGS_DATATYPE_STR8;			// STR	char[8]...
		else if (data_type.compare("INT1") == 0) type = AGS_DATATYPE_SINT8;		// SINT8 char...
		else if (data_type.compare("LOG1") == 0) type = AGS_DATATYPE_UINT8;		// UINT8 unsigned char...
		else if (data_type.compare("INT2") == 0) type = AGS_DATATYPE_SINT16;	// SINT16 short...
		else if (data_type.compare("LOG2") == 0) type = AGS_DATATYPE_UINT16;	// UINT16 unsigned short...
		else if (data_type.compare("INT4") == 0) type = AGS_DATATYPE_SINT32;	// SINT32 int...
		else if (data_type.compare("LOG4") == 0) type = AGS_DATATYPE_UINT32;	// UINT32 unsigned int...
		else if (data_type.compare("REAL") == 0) type = AGS_DATATYPE_REAL4;		// FLOAT32 float...
		else if (data_type.compare("DBLE") == 0) type = AGS_DATATYPE_REAL8;		// FLOAT64 double...
		else type = AGS_DATATYPE_INVALID;
	}
	catch (...)
	{
		// Log error...
		std::string msg = "ERROR: Unsupported type " + data_type + " from config file...";
		log(msg);
	}

	return type;
}

bool AGS_Server::find_section(const std::string& section_marker, std::ifstream& config_file)
{
	//bool found = true;
	bool found = false;
	std::string line;

	// Clear any failures and reset to start of file...
	config_file.clear();
	config_file.seekg(0);

	while (!found && getline(config_file, line))
	{
		if (line.compare(START_OF_LINE, section_marker.length(), section_marker) == 0)
		{
			found = true;
		}
	}

	return found;
}

bool AGS_Server::is_section_end(const std::string& line)
{
	// We've reached the end of a section if we hit a section end marker...
	// or we hit the marker for the start of a new section...

	bool section_end = false;
	if (line.compare(START_OF_LINE, CONNECT_MARKER.length(), CONNECT_MARKER) == 0
		|| line.compare(START_OF_LINE, PORT_MARKER.length(), PORT_MARKER) == 0
		|| line.compare(START_OF_LINE, ADDRESS_MARKER.length(), ADDRESS_MARKER) == 0
		|| line.compare(START_OF_LINE, INPUT_START_MARKER.length(), INPUT_START_MARKER) == 0
		|| line.compare(START_OF_LINE, INPUT_STOP_MARKER.length(), INPUT_STOP_MARKER) == 0
		|| line.compare(START_OF_LINE, OUTPUT_START_MARKER.length(), OUTPUT_START_MARKER) == 0
		|| line.compare(START_OF_LINE, OUTPUT_STOP_MARKER.length(), OUTPUT_STOP_MARKER) == 0
		)
	{
		section_end = true;
	}

	return section_end;
}

bool AGS_Server::is_comment(const std::string& line)
{
	// Check to see if this line is a comment...
	return (line.compare(START_OF_LINE, COMMENT_MARKER.length(), COMMENT_MARKER) == 0);
}

bool AGS_Server::is_whitespace(const std::string& line)
{
	// Check to see if this line is blank...
	return std::all_of(line.begin(), line.end(), std::isspace);
}

void AGS_Server::trim(std::string& line)
{
	// Trim all leading and trailing white space...
	line.erase(line.begin(), std::find_if(line.begin(), line.end(), [](signed char ch) {return !std::isspace(ch); }));
	line.erase(std::find_if(line.rbegin(), line.rend(), [](unsigned char ch) {return !std::isspace(ch); }).base(), line.end());
}