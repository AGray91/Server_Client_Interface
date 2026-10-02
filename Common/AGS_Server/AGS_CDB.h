#pragma once
#include <string>
#include "AGS_Server.h"
#include <unordered_map>
#include <vector>
#include <functional>
#include "AGS_Config.h"

namespace AGS_Server
{
	class CDB
	{
	public:
		CDB(Config_Rec& config);

		/*
		// Write Input to CDB...
		bool write_input(const std::string& label_name, const char value);				// sint8	INT1
		bool write_input(const std::string& label_name, const unsigned char value);		// uint8	LOG1
		bool write_input(const std::string& label_name, const short value);				// sint16	INT2
		bool write_input(const std::string& label_name, const unsigned short value);	// uint16	LOG2
		bool write_input(const std::string& label_name, const int value);				// sint32	INT4
		bool write_input(const std::string& label_name, const unsigned int value);		// uint32	LOG4
		bool write_input(const std::string& label_name, const float value);				// float32	REAL4
		bool write_input(const std::string& label_name, const double value);			// float64	DBLE

		// Read Input from CDB...
		bool read_input(const std::string& label_name, char& value);				// sint8	INT1
		bool read_input(const std::string& label_name, unsigned char& value);		// uint8	LOG1
		bool read_input(const std::string& label_name, short& value);				// sint16	INT2
		bool read_input(const std::string& label_name, unsigned short& value);		// uint16	LOG2
		bool read_input(const std::string& label_name, int& value);					// sint32	INT4
		bool read_input(const std::string& label_name, unsigned int& value);		// uint32	LOG4
		bool read_input(const std::string& label_name, float& value);				// float32	REAL4
		bool read_input(const std::string& label_name, double& value);				// float64	DBLE

		// Write Output to CDB...
		bool write_output(const std::string& label_name, const char value);				// sint8	INT1
		bool write_output(const std::string& label_name, const unsigned char value);	// uint8	LOG1
		bool write_output(const std::string& label_name, const short value);			// sint16	INT2
		bool write_output(const std::string& label_name, const unsigned short value);	// uint16	LOG2
		bool write_output(const std::string& label_name, const int value);				// sint32	INT4
		bool write_output(const std::string& label_name, const unsigned int value);		// uint32	LOG4
		bool write_output(const std::string& label_name, const float value);			// float32	REAL4
		bool write_output(const std::string& label_name, const double value);			// float64	DBLE

		// Read Output from CDB...
		bool read_output(const std::string& label_name, char& value);				// sint8	INT1
		bool read_output(const std::string& label_name, unsigned char& value);		// uint8	LOG1
		bool read_output(const std::string& label_name, short& value);				// sint16	INT2
		bool read_output(const std::string& label_name, unsigned short& value);		// uint16	LOG2
		bool read_output(const std::string& label_name, int& value);				// sint32	INT4
		bool read_output(const std::string& label_name, unsigned int& value);		// uint32	LOG4
		bool read_output(const std::string& label_name, float& value);				// float32	REAL4
		bool read_output(const std::string& label_name, double& value);				// float64	DBLE
		*/

		// Write Input to CDB...
		bool write_input(const AGS_DATA_RECORD& record);

		// Write Input Array to CDB...
		bool write_arr_input(const AGS_DATA_ARR_RECORD& record);

		// Read Input from CDB...
		bool read_input(const std::string& label_name, AGS_DATA_RECORD& value);

		// Read Input Array from CDB...
		bool read_arr_input(const std::string& label_name, AGS_DATA_ARR_RECORD& value);

		// Write Output to CDB...
		bool write_output(const AGS_DATA_RECORD& record);

		// Write Output Array to CDB...
		bool write_arr_output(const AGS_DATA_ARR_RECORD& record);

		// Read Output from CDB...
		bool read_output(const std::string& label_name, AGS_DATA_RECORD& value);

		// Read Output Array from CDB...
		bool read_arr_output(const std::string& label_name, AGS_DATA_ARR_RECORD& value);

		// Add a listener to CDB Updated Event...
		void add_listener_update(std::function<void()> event_listener);

		const std::vector<std::string> get_input_single_labels();
		const std::vector<std::string> get_input_arr_labels();
		const std::vector<std::string> get_output_single_labels();
		const std::vector<std::string> get_output_arr_labels();

	private:
		// Input data...
		std::unordered_map<std::string, AGS_DATA_RECORD> input_labels;
		std::unordered_map<std::string, AGS_DATA_ARR_RECORD> input_arr_labels;

		// Output data...
		std::unordered_map<std::string, AGS_DATA_RECORD> output_labels;
		std::unordered_map<std::string, AGS_DATA_ARR_RECORD> output_arr_labels;

		// Delegates to let outside systems react to CDB updated...
		std::vector<std::function<void()>> update_delegates;

	private:
		void add_input_single_label(const std::string label_name, const unsigned int label_type);
		void add_input_arr_label(const std::string label_name);
		void add_output_single_label(const std::string label_name, const unsigned int label_type);
		void add_output_arr_label(const std::string label_name);
	};
}